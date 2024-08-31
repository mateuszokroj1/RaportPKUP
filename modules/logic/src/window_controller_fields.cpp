#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>
#include <QtWidgets/QMessageBox>

// #include <tbb/tbb.h>

#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include "InputDataState.hpp"
#include "WindowController.hpp"

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
WindowController::WindowController(std::weak_ptr<Application> app)
	: _application(app), _addRepositoryCmd(new Command(this)), _searchForCommitsCmd(new Command(this)),
	  _presets_manager(this)
{
	if (auto app_ptr = _application.lock())
	{
		auto weak = app_ptr->get<IProcessFactory>();
		if (!(_process_factory = weak.lock()))
			return;

		_repository_detector = app_ptr->get<IRepositoryDetector>().lock();
	}

	const auto today = QDate::currentDate();
	_fromDay.setValue(QDate(today.year(), today.month(), 1));
	_toDay.setValue(QDate(today.year(), today.month(), today.daysInMonth()));

	//_addRepositoryCmd->bindableCanExecute().setBinding([this](){ return !_repositoryPath.value().isEmpty(); });
	connect(_addRepositoryCmd, &Command::onExecute, this, &WindowController::addRepository);

	connect(_searchForCommitsCmd, &Command::onExecute, this, &WindowController::searchForCommits);

	connect(this, &WindowController::commitsChanged, this, &WindowController::isFilteringEnabledChanged);

	connect(&_presets_manager, &PresetsManager::loaded, this, &WindowController::loadPresets, Qt::QueuedConnection);
	connect(this, &WindowController::presetsChanged, this, &WindowController::syncPresetsFile, Qt::QueuedConnection);

	connect(this, &WindowController::presetSelectorTextChanged, this, &WindowController::canSavePresetChanged);

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
	return _repositoryPath.value();
}

bool WindowController::canFetchBefore() const
{
	return _canFetchBefore.value();
}

bool WindowController::canSavePreset() const
{
	const auto text = presetSelectorText();

	return !text.isEmpty() &&
		   std::ranges::none_of(_presets, [&text](const Preset* item) { return item && item->name == text; });
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

	QUrl url(value);
	QDir dir(url.toLocalFile());
	if (!dir.exists())
	{
		_repositoryPath.setValue("");
		return;
	}

	if (!_repository_detector)
		return;

	const auto result = _repository_detector->detect(url.toLocalFile().toStdWString()).get();

	if (!result)
		return;

	_repositoryPath.setValue(QString::fromStdWString(result->generic_wstring()));
}

void WindowController::setCanFetchBefore(bool value)
{
	_canFetchBefore.setValue(value);
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

QBindable<QString> WindowController::bindableRepositoryPath() const
{
	return &_repositoryPath;
}

QBindable<bool> WindowController::bindableCanFetchBefore() const
{
	return &_canFetchBefore;
}

void WindowController::savePreset(const QString& name)
{
	Preset* preset = nullptr;

	for (auto item : _presets)
	{
		if (item->name == name)
			return;
	}

	if (!preset)
		preset = new Preset(this);

	preset->name = name;
	preset->authorName = _authorName.value();
	preset->authorEmail = _authorEmail.value();
	preset->city = _city.value();
	preset->repositories.clear();

	std::transform(_repositories.begin(), _repositories.end(), std::back_inserter(preset->repositories),
				   [](RepositoryListItem* item) { return item->path(); });

	_presets.push_back(preset);

	emit presetsChanged();
}

void WindowController::deletePreset(int index)
{
	if (index < 0)
		return;

	_presets.removeAt(index);

	emit presetsChanged();
}

void WindowController::recallPreset(int index)
{
	if (index < 0)
		return;

	const auto preset = _presets.at(index);

	setAuthorName(preset->authorName);
	setAuthorEmail(preset->authorEmail);
	setCity(preset->city);

	auto list = _repositories;
	qDeleteAll(list);
	_repositories.clear();

	const auto& accessor = _repository_detector->getAccessor();

	for (const auto& repoPath : preset->repositories)
	{
		const auto wstr = repoPath.toStdWString();

		if (auto repo = accessor.openRepository(std::filesystem::path(std::move(wstr))).get())
			_repositories.push_back(new RepositoryListItem(std::move(repo), this));
	}

	emit repositoriesChanged();
}

void WindowController::loadPresets()
{
	auto list = _presets;
	_presets.clear();
	qDeleteAll(list);

	std::copy(_presets_manager.presets.cbegin(), _presets_manager.presets.cend(), std::back_inserter(_presets));
	emit presetsChanged();
}

void WindowController::syncPresetsFile()
{
	if (_presets.size() == _presets_manager.presets.size())
	{
		bool broken = false;

		for (size_t i = 0; i < _presets.size(); ++i)
		{
			if (_presets[i] != _presets_manager.presets[i])
			{
				broken = true;
				break;
			}
		}

		if (!broken)
			return;
	}

	_presets_manager.presets.clear();
	std::copy(_presets.cbegin(), _presets.cend(), std::back_inserter(_presets_manager.presets));
	_presets_manager.saveToFile();
}

void WindowController::addRepository()
{
	const auto& accessor = _repository_detector->getAccessor();
	const auto wstr = repositoryPath().toStdWString();

	if (std::any_of(_repositories.cbegin(), _repositories.cend(),
					[this](const RepositoryListItem* item) { return item && item->path() == _repositoryPath; }))
		return;

	std::optional<Author> author;
	if (auto repo = accessor.openRepository(std::filesystem::path(std::move(wstr))).get())
	{
		author = repo->getDefaultAuthor();

		_repositories.push_back(new RepositoryListItem(std::move(repo), this));
		emit repositoriesChanged();
		_repositoryPath.setValue("");
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

void WindowController::removeCommit(CommitItem* ptr)
{
	const auto index = _commits.indexOf(ptr);
	if (index < 0 || index >= _commits.count())
		return;

	_commits.removeAt(index);
	ptr->deleteLater();

	emit repositoriesChanged();
}

bool WindowController::YesCancelDialog(const QString& title, const QString& message, const QString& detailed_info)
{
	return QMessageBox::warning(nullptr, title, message,
								QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::Cancel,
								QMessageBox::StandardButton::Yes) == QMessageBox::StandardButton::Yes;
}

} // namespace RaportPKUP::UI
