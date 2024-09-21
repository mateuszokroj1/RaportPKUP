#include "Process.hpp"

namespace RaportPKUP::UI
{
Process::Process(QObject* parent, QString&& command, QString&& working_directory)
	: _process(parent), _command(std::move(command))
{
	_process.setWorkingDirectory(std::move(working_directory));
}

Process::~Process() noexcept
{
	if (_process.state() == QProcess::ProcessState::Running)
		_process.kill();

	_process.close();
}

void Process::start()
{
	_process.startCommand(_command, QProcess::OpenMode{QProcess::OpenModeFlag::ReadOnly});
}

int Process::exitCode() const
{
	return _process.exitCode();
}

std::wstring Process::readOutput()
{
	const auto arr = _process.readAllStandardOutput();
	const QString string(arr);

	return string.toStdWString();
}

std::wstring Process::readError()
{
	const auto arr = _process.readAllStandardError();
	const QString string(arr);

	return string.toStdWString();
}

std::wstring Process::getCommand() const
{
	return _command.toStdWString();
}

std::wstring Process::getWorkingDirectory() const
{
	return {};
}

bool Process::waitForFinished(std::chrono::milliseconds timeout)
{
	return _process.waitForFinished(timeout.count());
}

bool Process::isFinished() const
{
	return _process.state() == QProcess::NotRunning;
}

bool Process::isError() const
{
	return _process.exitStatus() == QProcess::ExitStatus::CrashExit;
}

std::shared_ptr<IProcess> ProcessFactory::createNew(const std::wstring& command, const std::wstring& working_directory)
{
	if (auto application = _app.lock())
		if (auto qapp = application->getQApplication())
		{
			QString working_dir = QString::fromStdWString(working_directory);

			if (working_dir.isEmpty())
				working_dir = qapp->applicationDirPath();

			return std::make_shared<Process>(qapp.get(), QString::fromStdWString(command), std::move(working_dir));
		}

	return {};
}
} // namespace RaportPKUP::UI
