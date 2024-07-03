#include "Command.hpp"

namespace RaportPKUP::UI
{
Command::Command(QObject* parent) : QObject(parent)
{
}

bool Command::canExecute() const
{
	return _canExecute.value();
}

void Command::setCanExecute(bool new_value)
{
	_canExecute.setValue(new_value);
}

QBindable<bool> Command::bindableCanExecute() const
{
	return &_canExecute;
}

void Command::emitCanExecuteChanged()
{
	_canExecute.notify();
}

void Command::execute()
{
	if (_canExecute.value())
		emit onExecute();
}
} // namespace RaportPKUP::UI
