#include "WindowController.hpp"

#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>
#include <QtWidgets/QMessageBox>

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
WindowController::WindowController(std::weak_ptr<Application> app) : _application(app), _presets_manager(this)
{
	if (auto app_ptr = _application.lock())
	{
		auto weak = app_ptr->get<IProcessFactory>();
		if (!(_process_factory = weak.lock()))
		{
			qCritical() << "WindowController::ctor(): ProcessFactory is not available.";
			return;
		}

		_repository_detector = app_ptr->get<IRepositoryDetector>().lock();
		if (!_repository_detector)
		{
			qCritical() << "WindowController::ctor(): RepositoryDetector is not available.";
			return;
		}

		connect(app_ptr->getQApplication(), &QGuiApplication::aboutToQuit, this,
				[this]() { _application_exiting.request_stop(); });

		_connection_between_tokens = std::make_unique<std::stop_callback<std::function<void()>>>(
			_application_exiting.get_token(), [this] { _calculation_cancelled.request_stop(); });
	}
	else
		qCritical() << "WindowController::ctor(): Application pointer is null.";

	const auto today = QDate::currentDate();
	_fromDay.setValue(QDate(today.year(), today.month(), 1));
	_toDay.setValue(QDate(today.year(), today.month(), today.daysInMonth()));

	connect(&_presets_manager, &PresetsManager::loaded, this, &WindowController::loadPresets, Qt::QueuedConnection);
	connect(this, &WindowController::presetsChanged, this, &WindowController::syncPresetsFile, Qt::QueuedConnection);

	connect(this, &WindowController::presetSelectorTextChanged, this, &WindowController::canSavePresetChanged);

	connect(this, &WindowController::authorNameChanged, this, &WindowController::canStartSearchChanged);
	connect(this, &WindowController::authorEmailChanged, this, &WindowController::canStartSearchChanged);
	connect(this, &WindowController::repositoriesChanged, this, &WindowController::canStartSearchChanged);
	connect(this, &WindowController::fromDayChanged, this, &WindowController::canStartSearchChanged);
	connect(this, &WindowController::toDayChanged, this, &WindowController::canStartSearchChanged);

	connect(this, &WindowController::authorNameChanged, this, &WindowController::resetPreviewTimer);
	connect(this, &WindowController::cityChanged, this, &WindowController::resetPreviewTimer);
	connect(this, &WindowController::raportDateChanged, this, &WindowController::resetPreviewTimer);
	connect(this, &WindowController::fromDayChanged, this, &WindowController::resetPreviewTimer);
	connect(this, &WindowController::toDayChanged, this, &WindowController::resetPreviewTimer);

	connect(this, &WindowController::commitsChanged, this,
			[this]()
			{
				for (CommitItem* commit : _commits)
				{
					connect(commit, &CommitItem::durationChanged, this, &WindowController::sumOfHoursChanged);
					connect(commit, &CommitItem::durationChanged, this, &WindowController::resetPreviewTimer);
				}

				emit sumOfHoursChanged();
				resetPreviewTimer();
			});

	_timeout_for_preview.setSingleShot(true);
	_timeout_for_preview.setInterval(std::chrono::seconds(1));
	connect(&_timeout_for_preview, &QTimer::timeout, this, &WindowController::previewDocumentChanged);

	_raportDate.setBinding([this]() { return _toDay.value(); });

	_presets_manager.loadFromFile();
}

WindowController::~WindowController() noexcept
{
	qDeleteAll(_presets);
	qDeleteAll(_repositories);
	qDeleteAll(_commits);
}

QQmlListProperty<Preset> WindowController::presets()
{
	return QQmlListProperty<Preset>(this, &_presets);
}

QQmlListProperty<RepositoryListItem> WindowController::repositories()
{
	return QQmlListProperty<RepositoryListItem>(this, &_repositories);
}

QQmlListProperty<CommitItem> WindowController::commits()
{
	return QQmlListProperty<CommitItem>(this, &_commits);
}

QString WindowController::presetSelectorText() const
{
	return _presetSelectorText.value();
}

QString WindowController::authorName() const
{
	return _authorName.value();
}

QString WindowController::authorEmail() const
{
	return _authorEmail.value();
}

QString WindowController::city() const
{
	return _city.value();
}

QDate WindowController::fromDay() const
{
	return _fromDay.value();
}

QDate WindowController::toDay() const
{
	return _toDay.value();
}

QString WindowController::repositoryPath() const
{
	return _repository_path;
}

bool WindowController::canSavePreset() const
{
	const auto text = presetSelectorText();

	return !text.isEmpty() &&
		   std::ranges::none_of(_presets, [&text](const Preset* item) { return item && item->name == text; });
}

bool WindowController::canStartSearch() const
{
	if (authorName().isEmpty())
		return false;

	if (authorEmail().isEmpty())
		return false;

	if (fromDay() > toDay())
		return false;

	if (_repositories.empty())
		return false;

	return true;
}

QDate WindowController::raportDate() const
{
	return _raportDate.value();
}

void WindowController::setPresetSelectorText(QString value)
{
	_presetSelectorText.setValue(std::move(value));
}

void WindowController::setAuthorName(QString value)
{
	_authorName.setValue(std::move(value));
}

void WindowController::setAuthorEmail(QString value)
{
	_authorEmail.setValue(std::move(value));
}

void WindowController::setCity(QString value)
{
	_city.setValue(std::move(value));
}

void WindowController::setFromDay(QDate value)
{
	if (value > _toDay.value())
		value = _toDay.value();

	_fromDay.setValue(std::move(value));
}

void WindowController::setToDay(QDate value)
{
	if (value < _fromDay.value())
		value = _fromDay.value();

	_toDay.setValue(std::move(value));
}

void WindowController::setRepositoryPath(QString value)
{
	if (value.isEmpty())
		return;

	if (value.startsWith("file:///"))
		value = value.mid(8);

	QDir dir(value);
	if (!dir.exists())
	{
		_repository_path = {};
		emit repositoryPathChanged();
		return;
	}

	if (!_repository_detector)
		return;

	const auto result = _repository_detector->detect(dir.filesystemAbsolutePath()).get();

	if (!result)
		return;

	_repository_path = QString::fromStdWString(result->generic_wstring());
	emit repositoryPathChanged();
}

void WindowController::setRaportDate(QDate value)
{
	_raportDate.setValue(value);
}

QBindable<QString> WindowController::bindablePresetSelectorText() const
{
	return &_presetSelectorText;
}

QBindable<QString> WindowController::bindableAuthorName() const
{
	return &_authorName;
}

QBindable<QString> WindowController::bindableAuthorEmail() const
{
	return &_authorEmail;
}

QBindable<QString> WindowController::bindableCity() const
{
	return &_city;
}

QBindable<QDate> WindowController::bindableFromDay() const
{
	return &_fromDay;
}

QBindable<QDate> WindowController::bindableToDay() const
{
	return &_toDay;
}

QBindable<QDate> WindowController::bindableRaportDate() const
{
	return &_raportDate;
}

void WindowController::addRepository()
{
	const auto& accessor = _repository_detector->getAccessor();
	const auto wstr = repositoryPath().toStdWString();

	if (std::ranges::any_of(_repositories, [this](const RepositoryListItem* item)
							{ return item && item->path() == _repository_path; }))
		return;

	std::optional<Author> author;
	if (auto repo = accessor.openRepository(std::filesystem::path(std::move(wstr))).get())
	{
		author = repo->getDefaultAuthor();

		_repositories.push_back(new RepositoryListItem(std::move(repo), this));
		emit repositoriesChanged();
		_repository_path = {};
		emit repositoryPathChanged();
	}

	if (_repositories.count() == 1 && author)
	{
		setAuthorEmail(QString::fromStdWString(author->email));
		setAuthorName(QString::fromStdWString(author->name));
	}
}

void WindowController::removeRepository(int index)
{
	if (index < 0 || index >= _repositories.count())
		return;

	_repositories[index]->deleteLater();
	_repositories.removeAt(index);
	emit repositoriesChanged();
}

void WindowController::clearRepositories()
{
	qDeleteAll(_repositories);
	_repositories.clear();

	emit repositoriesChanged();
}

void WindowController::removeCommit(int index)
{
	if (index < 0 || index >= _commits.count())
		return;

	auto ptr = _commits[index];
	_commits.removeAt(index);
	ptr->deleteLater();

	emit commitsChanged();
}

uint WindowController::sumOfHours() const
{
	uint sum = 0;

	for (const auto& commit : _commits)
		sum += commit->duration;

	return sum;
}

void WindowController::resetPreviewTimer()
{
	_timeout_for_preview.stop();
	_timeout_for_preview.start();
}
} // namespace RaportPKUP::UI
