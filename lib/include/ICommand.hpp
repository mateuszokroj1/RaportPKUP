#pragma once

namespace RaportPKUP
{
class ICommand
{
  public:
	virtual ~ICommand() = default;

	virtual void execute() = 0;
	virtual bool canExecute() const = 0;
	virtual void setCanExecute(bool) const = 0;
	virtual void emitCanExecuteChanged() const = 0;
};
} // namespace RaportPKUP
