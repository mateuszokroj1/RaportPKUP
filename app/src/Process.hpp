#pragma once

#include <QProcess>

#include "Application.hpp"
#include <include/IProcess.hpp>

namespace RaportPKUP::UI
{
class Process final : public IProcess
{
  public:
	Process(QObject* parent, QString&& command, QString&& working_directory);
	~Process() noexcept override;

	void start() override;

	int exitCode() const override;
	std::wstring readOutput() override;
	std::wstring readError() override;
	std::vector<std::wstring> getArguments() const override;
	std::wstring getWorkingDirectory() const override;
	bool waitForFinished(const std::chrono::milliseconds&) override;

  private:
	QProcess _process;
	QString _command;
};

class ProcessFactory final : public IProcessFactory
{
  public:
	ProcessFactory(std::weak_ptr<Application> app) : _app(app)
	{
	}

	std::shared_ptr<IProcess> createNew(const std::wstring& command,
										const std::wstring& working_directory = {}) override;

  private:
	std::weak_ptr<Application> _app;
};
} // namespace RaportPKUP::UI
