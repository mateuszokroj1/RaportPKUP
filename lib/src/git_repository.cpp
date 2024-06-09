#include <chrono>
#include <codecvt>
#include <locale>
#include <regex>
#include <stdexcept>
#include <windows.h>

#include <GitRepository.hpp>
#include <base.hpp>

#include "process_runner.hpp"

namespace RaportPKUP
{
constexpr wchar_t log_column_separator_w = L'|';
constexpr char log_column_separator = '|';

GitRepository::GitRepository(const std::filesystem::path& path) : _dir(path)
{
	if (!checkIsValidPath(path))
		throw std::invalid_argument("path");
}

GitRepository::~GitRepository()
{
}

std::vector<std::string> GitRepository::getLog(const std::wstring& path, const std::string& from, const std::string& to,
											   const std::wstring& author)
{
	// git log --all --no-decorate --author=mateuszokroj1@gmail.com
	// --since='1-Jan-2023' --until='22-Feb-2024' --format='%h|~|~|%ci|~|~|%s'
	std::wstring cmd = L"git.exe log --all --no-decorate";
	cmd.append(L" --author=").append(author);
	cmd.append(L" --format=%h").append(L"|").append(L"%cs").append(L"|").append(L"%s");
	// cmd.append(L" --since='").append(std::move(std::wstring(from.cbegin(),
	// from.cend()))).append(L"'"); cmd.append(L"
	// --until='").append(std::move(std::wstring(to.cbegin(),
	// to.cend()))).append(L"'");

	DWORD last_error = 0;
	auto output = process_runner(cmd, path, last_error);

	if (last_error != 0)
	{
		return {};
	}

	std::vector<std::string> lines;
	std::string line;

	do
	{
		std::getline(output, line);

		if (!line.empty())
			lines.push_back(line);
	} while (!line.empty());

	return lines;
}

bool GitRepository::checkIsValidPath(const std::filesystem::path& path)
{
	DWORD err = 0;
	std::wstring cmd(L"git.exe status");
	auto output = process_runner(cmd, path.wstring(), err);

	if (err != 0)
		return {};

	std::string line;
	std::getline(output, line);
	return line.starts_with("On branch");
}

Author GitRepository::getSystemConfigAuthor()
{
	// git config --global user.email; git config --global user.name

	DWORD err = 0;
	std::wstring cmd(L"git.exe config --global user.email");

	auto output = process_runner(cmd, {}, err);

	if (err != 0)
		return {};

	std::string email;
	output >> email;

	cmd = L"git config --global user.name";
	output = process_runner(cmd, {}, err);

	if (err != 0)
		return {};

	std::string name;
	std::getline(output,
				 name); // TODO implement conversion UTF-8 to wchar, try WinAPI

	return {std::wstring(name.cbegin(), name.cend()), std::wstring(email.cbegin(), email.cend())};
}

std::shared_ptr<std::vector<Commit>> GitRepository::getCommitsFromTimeRange(
	const std::chrono::time_point<std::chrono::system_clock>& from,
	const std::chrono::time_point<std::chrono::system_clock>& to, const Author& author) const
{
	time_t from_t = std::chrono::system_clock::to_time_t(from);
	time_t to_t = std::chrono::system_clock::to_time_t(to);
	std::string from_str = std::ctime(&from_t);
	std::string to_str = std::ctime(&to_t);

	const auto commits =
		getLog(_dir, from_str.substr(0, from_str.size() - 1), to_str.substr(0, to_str.size() - 1), author.email);

	auto vector = std::make_shared<std::vector<Commit>>();
	for (const auto& line : commits)
	{
		Commit c;
		std::stringstream stream(line);

		{
			std::string id;
			std::getline(stream, id, '|');
			c.id = id;
		}
		{
			std::string datetime;
			std::getline(stream, datetime, '|');
			std::istringstream stream_date(datetime);

			std::chrono::from_stream(stream_date, "%Y-%m-%d", c.datetime);
		}

		std::getline(stream, c.message);

		c.author = author;

		vector->push_back(std::move(c));
	}

	return vector;
}
} // namespace RaportPKUP
