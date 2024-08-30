#pragma once

#include <QtCore/QDir>
#include <QtQml/QQmlListProperty>

#include <include/IProcess.hpp>
#include <include/IRepositoryDetector.hpp>

#include "Application.hpp"
#include "Command.hpp"
#include "CommitItem.hpp"
#include "Preset.hpp"
#include "PresetsManager.h"
#include "RepositoryListItem.hpp"

namespace RaportPKUP::UI
{
class WindowController : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_UNCREATABLE("Object from app controller")

  public:
	WindowController(std::weak_ptr<Application> app);
	~WindowController() noexcept override;

	/* Data input */
	Q_PROPERTY(QQmlListProperty<Preset> presets READ presets NOTIFY presetsChanged)

	Q_PROPERTY(
		QString authorName READ authorName WRITE setAuthorName NOTIFY authorNameChanged BINDABLE bindableAuthorName)
	Q_PROPERTY(QString authorEmail READ authorEmail WRITE setAuthorEmail NOTIFY authorEmailChanged BINDABLE
				   bindableAuthorEmail)
	Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged BINDABLE bindableCity)

	Q_PROPERTY(QDate fromDay READ fromDay WRITE setFromDay NOTIFY fromDayChanged BINDABLE bindableFromDay)
	Q_PROPERTY(QDate toDay READ toDay WRITE setToDay NOTIFY toDayChanged BINDABLE bindableToDay)

	Q_PROPERTY(QString repositoryPath READ repositoryPath WRITE setRepositoryPath NOTIFY repositoryPathChanged BINDABLE
				   bindableRepositoryPath)
	Q_PROPERTY(QQmlListProperty<RepositoryListItem> repositories READ repositories NOTIFY repositoriesChanged)

	Q_PROPERTY(bool canFetchBefore READ canFetchBefore WRITE setCanFetchBefore NOTIFY canFetchBeforeChanged BINDABLE
				   bindableCanFetchBefore)

	/* Filtering */
	Q_PROPERTY(QQmlListProperty<CommitItem> commits READ commits NOTIFY commitsChanged)

	Q_PROPERTY(QString previewDocument READ previewDocument NOTIFY previewDocumentChanged)

	Q_PROPERTY(Command* addRepositoryCmd READ addRepositoryCmd CONSTANT)
	Q_PROPERTY(Command* searchForCommitsCmd READ searchForCommitsCmd CONSTANT)

	QQmlListProperty<Preset> presets();
	QQmlListProperty<RepositoryListItem> repositories();
	QQmlListProperty<CommitItem> commits();

	QString authorName() const;
	QString authorEmail() const;
	QString city() const;
	QDate fromDay() const;
	QDate toDay() const;
	QString repositoryPath() const;
	bool canFetchBefore() const;

	QString raportFileName() const;

	QString previewDocument() const
	{
		return {}; // TODO
	}

	Command* addRepositoryCmd() const
	{
		return _addRepositoryCmd;
	}

	Command* searchForCommitsCmd() const
	{
		return _searchForCommitsCmd;
	}

	void setAuthorName(QString);
	void setAuthorEmail(QString);
	void setCity(QString);
	void setFromDay(QDate);
	void setToDay(QDate);
	Q_INVOKABLE void setRepositoryPath(QString);
	void setCanFetchBefore(bool);

	QBindable<QString> bindableAuthorName() const;
	QBindable<QString> bindableAuthorEmail() const;
	QBindable<QString> bindableCity() const;
	QBindable<QDate> bindableFromDay() const;
	QBindable<QDate> bindableToDay() const;
	QBindable<QString> bindableRepositoryPath() const;
	QBindable<bool> bindableCanFetchBefore() const;

  signals:
	void itemsChanged();
	void presetsChanged();
	void authorNameChanged();
	void authorEmailChanged();
	void cityChanged();
	void fromDayChanged();
	void toDayChanged();
	void repositoriesChanged();
	void repositoryPathChanged();
	void canFetchBeforeChanged();
	void isFilteringEnabledChanged();
	void commitsChanged();
	void previewDocumentChanged();

  public:
	Q_INVOKABLE void savePreset(const QString&);
	Q_INVOKABLE void deletePreset(int index);
	Q_INVOKABLE void recallPreset(int index);

	Q_INVOKABLE void addRepository();
	Q_INVOKABLE void removeRepository(int index);
	Q_INVOKABLE void clearRepositories();

	Q_INVOKABLE void searchForCommits();
	Q_INVOKABLE void removeCommit(CommitItem*);

	Q_INVOKABLE void saveRaportToFile(QString filename);

  private:
	void loadPresets();

	std::weak_ptr<Application> _application;
	QList<Preset*> _presets;
	QList<RepositoryListItem*> _repositories;
	QList<CommitItem*> _commits;
	std::shared_ptr<IProcessFactory> _process_factory;
	std::shared_ptr<IRepositoryDetector> _repository_detector;
	PresetsManager _presets_manager;

	Command* _addRepositoryCmd = nullptr;
	Command* _searchForCommitsCmd = nullptr;

	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _authorName, &WindowController::authorNameChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _authorEmail, &WindowController::authorEmailChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _city, &WindowController::cityChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDate, _fromDay, &WindowController::fromDayChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDate, _toDay, &WindowController::toDayChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _repositoryPath, &WindowController::repositoryPathChanged)
	Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(WindowController, bool, _canFetchBefore, true,
										 &WindowController::canFetchBeforeChanged)
};
} // namespace RaportPKUP::UI
