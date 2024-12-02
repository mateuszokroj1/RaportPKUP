#pragma once

#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtQml/QQmlListProperty>

#include <include/IProcess.hpp>
#include <include/IRepositoryDetector.hpp>

#include "Application.hpp"
#include "Command.hpp"
#include "CommitItem.hpp"
#include "Preset.hpp"
#include "PresetsManager.hpp"
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

	Q_PROPERTY(QString presetSelectorText READ presetSelectorText WRITE setPresetSelectorText NOTIFY
				   presetSelectorTextChanged BINDABLE bindablePresetSelectorText)
	Q_PROPERTY(
		QString authorName READ authorName WRITE setAuthorName NOTIFY authorNameChanged BINDABLE bindableAuthorName)
	Q_PROPERTY(QString authorEmail READ authorEmail WRITE setAuthorEmail NOTIFY authorEmailChanged BINDABLE
				   bindableAuthorEmail)
	Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged BINDABLE bindableCity)

	Q_PROPERTY(QDate fromDay READ fromDay WRITE setFromDay NOTIFY fromDayChanged BINDABLE bindableFromDay)
	Q_PROPERTY(QDate toDay READ toDay WRITE setToDay NOTIFY toDayChanged BINDABLE bindableToDay)

	Q_PROPERTY(QString repositoryPath READ repositoryPath WRITE setRepositoryPath NOTIFY repositoryPathChanged)
	Q_PROPERTY(QQmlListProperty<RepositoryListItem> repositories READ repositories NOTIFY repositoriesChanged)

	Q_PROPERTY(bool canSavePreset READ canSavePreset NOTIFY canSavePresetChanged)
	Q_PROPERTY(bool canStartSearch READ canStartSearch NOTIFY canStartSearchChanged)

	/* Filtering */
	Q_PROPERTY(QQmlListProperty<CommitItem> commits READ commits NOTIFY commitsChanged)
	Q_PROPERTY(uint sumOfHours READ sumOfHours NOTIFY sumOfHoursChanged)

	Q_PROPERTY(
		QDate raportDate READ raportDate WRITE setRaportDate NOTIFY raportDateChanged BINDABLE bindableRaportDate)
	Q_PROPERTY(QString previewDocument READ previewDocument NOTIFY previewDocumentChanged)

	QQmlListProperty<Preset> presets();
	QQmlListProperty<RepositoryListItem> repositories();
	QQmlListProperty<CommitItem> commits();

	QString presetSelectorText() const;
	QString authorName() const;
	QString authorEmail() const;
	QString city() const;
	QDate fromDay() const;
	QDate toDay() const;
	QString repositoryPath() const;
	bool canSavePreset() const;
	bool canStartSearch() const;

	QString previewDocument() const;

	QDate raportDate() const;

	void setPresetSelectorText(QString);
	void setAuthorName(QString);
	void setAuthorEmail(QString);
	void setCity(QString);
	void setFromDay(QDate);
	void setToDay(QDate);
	Q_INVOKABLE void setRepositoryPath(QString);
	void setRaportDate(QDate);

	QBindable<QString> bindablePresetSelectorText() const;
	QBindable<QString> bindableAuthorName() const;
	QBindable<QString> bindableAuthorEmail() const;
	QBindable<QString> bindableCity() const;
	QBindable<QDate> bindableFromDay() const;
	QBindable<QDate> bindableToDay() const;
	QBindable<QDate> bindableRaportDate() const;

  signals:
	void itemsChanged();
	void presetsChanged();
	void presetSelectorTextChanged();
	void authorNameChanged();
	void authorEmailChanged();
	void cityChanged();
	void fromDayChanged();
	void toDayChanged();
	void repositoriesChanged();
	void repositoryPathChanged();
	void canStartSearchChanged();
	void canSavePresetChanged();

	void commitsChanged();
	void sumOfHoursChanged();

	void lockScreen1();
	void lockScreen2();
	void showWarning(QString msg);
	void unlockScreen();
	void showFilteringView();

	void raportDateChanged();
	void previewDocumentChanged();

  public:
	Q_INVOKABLE void savePreset(const QString&);
	Q_INVOKABLE void deletePreset(int index);
	Q_INVOKABLE void recallPreset(int index);

	Q_INVOKABLE void addRepository();
	Q_INVOKABLE void removeRepository(int index);
	Q_INVOKABLE void clearRepositories();

	Q_INVOKABLE void searchForCommits();
	Q_INVOKABLE void removeCommit(int index);

	Q_INVOKABLE uint sumOfHours() const;

	Q_INVOKABLE void saveRaportToFile(QString filename);

  private:
	void loadPresets();
	void syncPresetsFile();
	void resetPreviewTimer();
	void updateMiktex();

	std::weak_ptr<Application> _application;
	std::shared_ptr<IProcessFactory> _process_factory;
	std::shared_ptr<IRepositoryDetector> _repository_detector;
	PresetsManager _presets_manager;

	QList<Preset*> _presets;
	QList<RepositoryListItem*> _repositories;
	std::map<Commit::Key, Commit> _commits_temp;
	QList<CommitItem*> _commits;

	QString _repository_path;

	QTimer _timeout_for_preview;
	std::atomic_bool _thread_finished = false;
	std::stop_source _calculation_cancelled;
	std::stop_source _application_exiting;
	std::unique_ptr<std::stop_callback<std::function<void()>>> _connection_between_tokens;
	std::thread _update_miktex_thread;

	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _presetSelectorText,
							   &WindowController::presetSelectorTextChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _authorName, &WindowController::authorNameChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _authorEmail, &WindowController::authorEmailChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QString, _city, &WindowController::cityChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDate, _fromDay, &WindowController::fromDayChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDate, _toDay, &WindowController::toDayChanged)
	Q_OBJECT_BINDABLE_PROPERTY(WindowController, QDate, _raportDate, &WindowController::raportDateChanged)
};
} // namespace RaportPKUP::UI
