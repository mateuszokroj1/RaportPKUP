#pragma once

#include "base.hpp"
#include <chrono>
#include <string>
#include <vector>

using namespace std::chrono_literals;

namespace RaportPKUP
{
class IProcess
{
  public:
	virtual ~IProcess() = default;

	virtual void start() = 0;

	virtual int exitCode() const = 0;
	virtual std::wstring readOutput() = 0;
	virtual std::wstring readError() = 0;
	virtual std::wstring getCommand() const = 0;
	virtual bool isFinished() const = 0;
	virtual bool isError() const = 0;
	virtual std::wstring getWorkingDirectory() const = 0;
	virtual bool waitForFinished(std::chrono::milliseconds = 30s) = 0;
};

class IProcessFactory : public ICastable
{
  public:
	virtual ~IProcessFactory() = default;

	virtual std::shared_ptr<IProcess> createNew(const std::wstring& command,
												const std::wstring& working_directory = {}) = 0;
};
} // namespace RaportPKUP
