#include <tbb/parallel_for_each.h>

#include "include/base.hpp"
#include "include/GitRepository.hpp"

namespace RaportGen
{
    GitRepository::GitRepository(const std::filesystem::path &path)
    {
    }

    std::future<bool> GitRepository::checkIsValidPath(const std::filesystem::path &path)
    {
    }

    std::future<Author> GitRepository::getSystemConfigAuthor()
    {
    }

    std::shared_ptr<std::vector<Commit>> GitRepository::getCommitsFromTimeRange(const std::chrono::time_point<std::chrono::system_clock> &from, std::optional<std::chrono::time_point<std::chrono::system_clock>> to, std::optional<Author> author) const
    {
    }
}