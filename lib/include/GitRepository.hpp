#pragma once

#include <filesystem>
#include <future>

#include "IRepository.hpp"

namespace RaportGen
{
    class RAPORTGENCORE_EXPORT GitRepository : public IRepository
    {
    public:
        explicit GitRepository(const std::filesystem::path &);
        ~GitRepository() override {}

    private:
        GitRepository() = default;

    public:
        static std::future<bool> checkIsValidPath(const std::filesystem::path &);
        static std::future<Author> getSystemConfigAuthor();

        std::shared_ptr<std::vector<Commit>> getCommitsFromTimeRange(const std::chrono::time_point<std::chrono::system_clock>& from, std::optional<std::chrono::time_point<std::chrono::system_clock>> to = {}, std::optional<Author> author = {}) const override;
    };
}