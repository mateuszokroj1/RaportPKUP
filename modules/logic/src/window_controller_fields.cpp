#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>

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
	connect(this, &WindowController::presetsChanged, &_presets_manager, &PresetsManager::saveToFile,
			Qt::QueuedConnection);

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
		{
			preset = item;
			break;
		}
	}

	if (!preset)
		preset = new Preset(this);

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
	const auto preset = _presets.at(index);

	// setAuthorName(); TODO
}

void WindowController::loadPresets()
{
	// TODO
}

void WindowController::addRepository()
{
	const auto& accessor = _repository_detector->getAccessor();
	const auto wstr = repositoryPath().toStdWString();

	// TODO find duplicate
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

} // namespace RaportPKUP::UI
