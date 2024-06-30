#pragma once

namespace RaportPKUP
{
class ICommand
{
  public:
	virtual ~ICommand() = default;

	virtual void execute() = 0;
	virtual bool canExecute() const = 0;
	virtual void setCanExecute(bool) = 0;
	virtual void emitCanExecuteChanged() = 0;
};
} // namespace RaportPKUP
