#include <QtQml/qqmllist.h>
#include <QtWidgets/QFileDialog>

#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include "WindowController.hpp"

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
WindowController::WindowController(std::weak_ptr<Application> app) : _application(app)
{
	if (auto app_ptr = _application.lock())
	{
		auto weak = app_ptr->get<IProcessFactory>();
		if (!(_process_factory = weak.lock()))
			return;

		_repository_detector = app_ptr->get<IRepositoryDetector>().lock();

		if (auto qml_engine = app_ptr->getQmlEngine())
			creatingSteps(qml_engine);
	}

	const auto today = QDate::currentDate();
	_fromDay.setValue(QDate(today.year(), today.month(), 1));
	_toDay.setValue(QDate(today.year(), today.month(), today.daysInMonth()));
}

WindowController::~WindowController() noexcept
{
	qDeleteAll(_items_list);
	qDeleteAll(_repositories);
	qDeleteAll(_commits);
}

void WindowController::creatingSteps(QQmlApplicationEngine* qml)
{
	auto item = new MainViewItem(this);
	{
		item->setName(u"Wprowadzanie danych"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/DataInputStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem*>(component.create());
		auto errors = component.errors();

		if (view && errors.empty())
		{
			item->setContent(view);

			_items_list.push_back(item);
		}
	}

	{
		item = new MainViewItem(this);
		item->setName(u"Uzupełnianie godzin"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/DataFilteringStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem*>(component.create());
		auto errors = component.errors();

		if (view && errors.empty())
		{
			item->setContent(view);

			_items_list.push_back(item);
		}
	}

	{
		item = new MainViewItem(this);
		item->setName(u"Generowanie raportu"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/ReportingStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem*>(component.create());
		auto errors = component.errors();

		if (view && errors.empty())
		{
			item->setContent(view);

			_items_list.push_back(item);
		}
	}

	_items = QQmlListProperty<MainViewItem>(this, &_items_list);
	emit itemsChanged();
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

QDate WindowController::fromDay() const
{
	return _fromDay.value();
}

QDate WindowController::toDay() const
{
	return _toDay.value();
}

QDir WindowController::repositoryPath() const
{
	return _repositoryPath.value();
}

bool WindowController::canFetchBefore() const
{
	return _canFetchBefore.value();
}

bool WindowController::isFilteringEnabled() const
{
	return _commits.count();
}

void WindowController::setAuthorName(QString value)
{
	_authorName.setValue(std::move(value));
}

void WindowController::setAuthorEmail(QString value)
{
	_authorEmail.setValue(std::move(value));
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

void WindowController::setRepositoryPath(QDir value)
{
	_repositoryPath.setValue(value);
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

QBindable<QDate> WindowController::bindableFromDay() const
{
	return &_fromDay;
}

QBindable<QDate> WindowController::bindableToDay() const
{
	return &_toDay;
}

QBindable<QDir> WindowController::bindableRepositoryPath() const
{
	return &_repositoryPath;
}

QBindable<bool> WindowController::bindableCanFetchBefore() const
{
	return &_canFetchBefore;
}

void WindowController::browseForRepository()
{
	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setViewMode(QFileDialog::ViewMode::List);
	dialog.setOptions({QFileDialog::Option::ShowDirsOnly});
	dialog.setModal(true);

	if (dialog.exec() != QDialog::DialogCode::Accepted)
		return;

	const auto accessor = _repository_detector->getAccessor();

	const auto dir = dialog.directory();
	if (auto corrected_dir = _repository_detector->detect(dir.filesystemPath()).get())
	{
		if (auto repo = accessor.openRepository(*corrected_dir).get())
		{
			//_repositories.push_back(new RepositoryListItem(repo)); TODO
			emit repositoriesChanged();
		}
	}
}

void WindowController::addRepository()
{
}

void WindowController::removeRepository(RepositoryListItem*)
{
}

void WindowController::clearRepositories()
{
}

void WindowController::searchForCommits()
{
}

void WindowController::removeCommit(CommitItem*)
{
}

void WindowController::saveRaportToFile()
{
}
} // namespace RaportPKUP::UI
