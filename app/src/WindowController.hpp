#pragma once

#include <QtCore/QDir>
#include <QtQml/QQmlListProperty>

#include <include/IProcess.hpp>
#include <include/IRepositoryDetector.hpp>

#include "Application.hpp"
#include "CommitItem.hpp"
#include "MainViewItem.hpp"
#include "RepositoryListItem.hpp"

namespace RaportPKUP::UI
{
class WindowController : public QObject
{
	Q_OBJECT

  public:
	WindowController(std::weak_ptr<Application> app);

	Q_PROPERTY(QQmlListProperty<MainViewItem> items READ items NOTIFY itemsChanged)

	/* Data input */
	Q_PROPERTY(
		QString authorName READ authorName WRITE setAuthorName NOTIFY authorNameChanged BINDABLE bindableAuthorName)
	Q_PROPERTY(QString authorEmail READ authorEmail WRITE setAuthorEmail NOTIFY authorEmailChanged BINDABLE
				   bindableAuthorEmail)

	Q_PROPERTY(QDate fromDay READ fromDay WRITE setFromDay NOTIFY fromDayChanged BINDABLE bindableFromDay)
	Q_PROPERTY(QDate toDay READ toDay WRITE setToDay NOTIFY toDayChanged BINDABLE bindableToDay)

	Q_PROPERTY(QDir repositoryPath READ repositoryPath WRITE setRepositoryPath NOTIFY repositoryPathChanged BINDABLE
				   bindableRepositoryPath)
	Q_PROPERTY(QQmlListProperty<RepositoryListItem> repositories READ repositories NOTIFY repositoriesChanged)

	Q_PROPERTY(bool canFetchBefore READ canFetchBefore WRITE setCanFetchBefore NOTIFY canFetchBeforeChanged BINDABLE
				   bindableCanFetchBefore)

	/* Filtering */
	Q_PROPERTY(bool isFilteringEnabled READ isFilteringEnabled NOTIFY isFilteringEnabledChanged BINDABLE
				   bindableIsFilteringEnabled)
	Q_PROPERTY(QQmlListProperty<CommitItem> commits READ commits NOTIFY commitsChanged)

	QQmlListProperty<MainViewItem> items() const
	{
		return _items;
	}

	QQmlListProperty<RepositoryListItem> repositories() const;
	QQmlListProperty<CommitItem> commits() const;

	QString authorName() const;
	QString authorEmail() const;
	QDate fromDay() const;
	QDate toDay() const;
	QDir repositoryPath() const;
	bool canFetchBefore() const;

	bool isFilteringEnabled() const;

	void setAuthorName(QString);
	void setAuthorEmail(QString);
	void setFromDay(QDate);
	void setToDay(QDate);
	void setRepositoryPath(QDir);
	void setCanFetchBefore(bool);

	QBindable<QString> bindableAuthorName() const;
	QBindable<QString> bindableAuthorEmail() const;
	QBindable<QDate> bindableFromDay() const;
	QBindable<QDate> bindableToDay() const;
	QBindable<QDir> bindableRepositoryPath() const;
	QBindable<bool> bindableCanFetchBefore() const;
	QBindable<bool> bindableIsFilteringEnabled() const;

  signals:
	void itemsChanged();
	void authorNameChanged();
	void authorEmailChanged();
	void fromDayChanged();
	void toDayChanged();
	void repositoriesChanged();
	void repositoryPathChanged();
	void canFetchBeforeChanged();

	void isFilteringEnabledChanged();

	void commitsChanged();

	Q_INVOKABLE void addRepository();

	Q_INVOKABLE void searchForCommits();

  private:
	void creatingSteps(QQmlApplicationEngine* qml);

	std::weak_ptr<Application> _application;
	QList<MainViewItem*> _qlist;
	QQmlListProperty<MainViewItem> _items;
	std::shared_ptr<IProcessFactory> _process_factory;
	std::shared_ptr<IRepositoryDetector> _repository_detector;

	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _authorName, &authorNameChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _authorEmail, &authorEmailChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDate, _fromDay, &fromDayChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDate, _toDay, &toDayChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDir, _repositoryPath, &repositoriesChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, bool, _canFetchBefore, &canFetchBeforeChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, bool, _isFilteringEnabled, &isFilteringEnabledChanged)
};
} // namespace RaportPKUP::UI
