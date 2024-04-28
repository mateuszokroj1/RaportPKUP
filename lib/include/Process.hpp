#pragma once

namespace RaportGen
{
class IProcess
{
  public:
	virtual ~IProcess() = default;

	void start() = 0;

	int exitCode() const = 0;
	std::wstring readOutput() = 0;
	std::wstring readError() = 0;
	std::vector<std::wstring> getArguments() const = 0;
	std::wstring getWorkingDirectory() const = 0;
	bool waitForFinished(const std::chrono::milliseconds &) = 0;
};

class IProcessFactory
{
  public:
	virtual ~IProcessFactory() = default;

	std::shared_ptr<IProcess> createNew(const std::wstring &command, const std::wstring &working_directory = {}) = 0;
};
} // namespace RaportGen
