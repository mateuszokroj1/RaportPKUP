#pragma once

#include <QtCore/QBindable>
#include <QtCore/QObject>
#include <QtQml/qqmlregistration.h>

#include <include/ICommand.hpp>

namespace RaportPKUP::UI
{
class Command : public QObject, public ICommand
{
	Q_OBJECT
	QML_ELEMENT
	Q_PROPERTY(bool canExecute READ canExecute WRITE setCanExecute NOTIFY canExecuteChanged DESIGNABLE true FINAL)

  public:
	Command(QObject* parent = nullptr);

	bool canExecute() const override;
	void setCanExecute(bool new_value) override;
	QBindable<bool> bindableCanExecute() const;

	void emitCanExecuteChanged() override;

  signals:

	void canExecuteChanged();
	void onExecute();

  public slots:

	Q_INVOKABLE void execute() override;

  private:
	Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(Command, bool, _canExecute, true, &Command::canExecuteChanged)
};
} // namespace RaportPKUP::UI
