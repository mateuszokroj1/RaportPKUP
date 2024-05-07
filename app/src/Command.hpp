#pragma once

#include <QtCore/QBindable>
#include <QtCore/QObject>

#include <include/ICommand.hpp>

namespace RaportPKUP
{
class Command : public QObject, public RaportGen::ICommand
{
	Q_OBJECT
	Q_PROPERTY(bool canExecute READ canExecute WRITE setCanExecute NOTIFY canExecuteChanged DESIGNABLE true FINAL)

  public:
	Command(const std::function<void()> executor);

	bool canExecute() const override;
	void setCanExecute(bool new_value) const override;
	QBindable<bool> bindableCanExecute() const;

	void emitCanExecuteChanged() const override;

  signals:

	void canExecuteChanged();

  public slots:

	Q_INVOKABLE void execute() override;

  private:
	Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(Command, bool, _canExecute, true, &Command::canExecuteChanged)
};
} // namespace RaportPKUP
