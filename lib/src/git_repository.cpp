#include <tbb/parallel_for_each.h>
#include <git2.h>
#include <windows.h>
#include <regex>
#include <stdexcept>
#include <string_view>
#include <codecvt>
#include <locale>

#include <base.hpp>
#include <GitRepository.hpp>

#include "libgit_wrapper.hpp"
#include "process_runner.hpp"

namespace RaportGen
{
    constexpr wchar_t log_column_separator[] = L"|~|~|";

    GitRepository::GitRepository(const std::filesystem::path &path) : _dir(path)
    {
        if (!checkIsValidPath(path))
            throw std::invalid_argument("path");


    }

    GitRepository::~GitRepository()
    {
    }

    std::vector<std::string> GitRepository::getLog(const std::wstring& path, const std::string& from, const std::string& to, const std::wstring& author)
    {
        //git log --all --no-decorate --author=mateuszokroj1@gmail.com --since='1-Jan-2023' --until='22-Feb-2024' --format='%h|~|~|%ci|~|~|%s'
        std::wstring cmd = L"git.exe log --all --no-decorate";
        cmd.append(L" --author=").append(author);
        cmd.append(L" --format = '%h").append(log_column_separator).append(L"%ci").append(log_column_separator).append(L"%s'");
        cmd.append(L" --since='").append(std::move(std::wstring(from.cbegin(), from.cend()))).append(L"'");
        cmd.append(L" --until='").append(std::move(std::wstring(to.cbegin(), to.cend()))).append(L"'");

        DWORD last_error = 0;
        auto output = process_runner(cmd, path, last_error);

        if (last_error != 0)
        {
            return {};
        }

        std::vector<std::string> lines;
        std::string line;
        while (output >> line)
            lines.push_back(line);

        return lines;
    }

    bool GitRepository::checkIsValidPath(const std::filesystem::path &path)
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
        //git config --global user.email; git config --global user.name

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
        std::getline(output, name); //TODO implement conversion UTF-8 to wchar

        return {std::wstring(name.cbegin(), name.cend()), std::wstring(email.cbegin(), email.cend())};
    }

    std::shared_ptr<std::vector<Commit>> GitRepository::getCommitsFromTimeRange(const std::chrono::time_point<std::chrono::system_clock>& from, const std::chrono::time_point<std::chrono::system_clock>& to, const Author& author) const
    {
        const auto commits = getLog(_dir, std::format("%Y-%m-%d", from), std::format("%Y-%m-%d", to), author.email);

    }
}