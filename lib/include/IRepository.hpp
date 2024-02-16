#pragma once

#include <vector>
#include <memory>
#include <chrono>

#include "Commit.hpp"
#include "RaportGenCoreExport.h"
#include <optional>

namespace RaportGen
{
    class RAPORTGENCORE_EXPORT IRepository
    {
    protected:
        IRepository() = default;
    
    public:
        IRepository(const IRepository&) = delete;
        IRepository(IRepository&&) = delete;
        virtual ~IRepository() {}

        virtual std::shared_ptr<std::vector<Commit>> getCommitsFromTimeRange(const std::chrono::time_point<std::chrono::system_clock>& from, std::optional<std::chrono::time_point<std::chrono::system_clock>> to = {}, std::optional<Author> author = {}) const = 0;
    };
}