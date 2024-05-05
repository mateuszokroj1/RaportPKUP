#pragma once

#include "base.hpp"
#include <chrono>
#include <string>
#include <vector>

namespace RaportGen
{
class IProcess
{
  public:
	virtual ~IProcess() = default;

	virtual void start() = 0;

	virtual int exitCode() const = 0;
	virtual std::wstring readOutput() = 0;
	virtual std::wstring readError() = 0;
	virtual std::vector<std::wstring> getArguments() const = 0;
	virtual std::wstring getWorkingDirectory() const = 0;
	virtual bool waitForFinished(const std::chrono::milliseconds &) = 0;
};

class IProcessFactory : public ICastable
{
  public:
	virtual ~IProcessFactory() = default;

	virtual std::shared_ptr<IProcess> createNew(const std::wstring &command,
												const std::wstring &working_directory = {}) = 0;
};
} // namespace RaportGen
