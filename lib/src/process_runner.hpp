#pragma once

#include <string>
#include <sstream>

namespace RaportGen
{
	std::stringstream process_runner(std::wstring& command_line, const std::wstring& working_directory, DWORD& last_error);
}