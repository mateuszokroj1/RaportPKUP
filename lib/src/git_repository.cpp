#include <tbb/parallel_for_each.h>
#include <git2.h>

#include <base.hpp>
#include <GitRepository.hpp>

#include "libgit_wrapper.hpp"

namespace RaportGen
{
    GitRepository::GitRepository(const std::filesystem::path &path)
    {
    }

    GitRepository::~GitRepository()
    {
        
    }

    std::future<bool> GitRepository::checkIsValidPath(const std::filesystem::path &path)
    {
const auto git_api = GitAPI::
    }

    std::future<Author> GitRepository::getSystemConfigAuthor()
    {
    }

    std::shared_ptr<std::vector<Commit>> GitRepository::getCommitsFromTimeRange(const std::chrono::time_point<std::chrono::system_clock> &from, std::optional<std::chrono::time_point<std::chrono::system_clock>> to, std::optional<Author> author) const
    {
    }
}