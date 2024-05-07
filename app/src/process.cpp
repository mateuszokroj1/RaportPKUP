#include "Process.hpp"

namespace RaportPKUP::UI
{
Process::Process(QObject *parent, QString &&command, QString &&working_directory) : _process(parent)
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
	return {};
}

std::wstring Process::readError()
{
	return {};
}

std::vector<std::wstring> Process::getArguments() const
{
	return {};
}

std::wstring Process::getWorkingDirectory() const
{
	return {};
}

bool Process::waitForFinished(const std::chrono::milliseconds &timeout)
{
	return _process.waitForFinished(timeout.count());
}

std::shared_ptr<IProcess> ProcessFactory::createNew(const std::wstring &command, const std::wstring &working_directory)
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
