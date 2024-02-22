#include "handle.hpp"

#include "process_runner.hpp"

namespace RaportGen
{
	std::stringstream process_runner(std::wstring& command_line, const std::wstring& working_directory, DWORD& last_error)
	{
		Handle outPipe_writer;
		Handle outPipe_reader;

		SECURITY_ATTRIBUTES attributes;
		attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		attributes.bInheritHandle = true;
		attributes.lpSecurityDescriptor = nullptr;

		if (!CreatePipe(outPipe_reader.ptr(), outPipe_writer.ptr(), &attributes, 0))
		{
			last_error = GetLastError();
			SetLastError(0);
			return {};
		}

		STARTUPINFOW parameters;
		ZeroMemory(&parameters, sizeof(parameters));
		parameters.cb = sizeof(parameters);
		parameters.hStdOutput = outPipe_writer;
		parameters.dwFlags = STARTF_USESTDHANDLES;

		PROCESS_INFORMATION result;
		ZeroMemory(&result, sizeof(result));
		if (!CreateProcessW(nullptr, command_line.data(), nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, !working_directory.empty() ? working_directory.c_str() : nullptr, &parameters, &result))
		{
			last_error = GetLastError();
			SetLastError(0);
			return {};
		}

		// Wait until child process exits.
		WaitForSingleObject(result.hProcess, INFINITE);

		// Close process and thread handles.
		CloseHandle(result.hProcess);
		CloseHandle(result.hThread);

		outPipe_writer.release();

		constexpr size_t buffer_size = 1024;
		char buffer[buffer_size];
		std::stringstream output;
		bool success = false;

		do
		{
			ZeroMemory(&buffer, buffer_size);

			DWORD readed = 0;
			success = ReadFile(outPipe_reader, &buffer, buffer_size, &readed, nullptr);
			if (readed > 0)
				output << buffer;

		} while (success);

		output.flush();

		if(output.gcount() > 0)
			output.seekg(output.beg);

		return output;
	}
}