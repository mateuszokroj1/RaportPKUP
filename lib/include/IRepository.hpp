#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <optional>

#include "Commit.hpp"

namespace RaportGen
{
    class IRepository
    {
    protected:
        IRepository() = default;
    
    public:
        IRepository(const IRepository&) = delete;
        IRepository(IRepository&&) = delete;
        virtual ~IRepository() {}

        virtual std::shared_ptr<std::vector<Commit>> getCommitsFromTimeRange(const std::chrono::time_point<std::chrono::system_clock>& from, const std::chrono::time_point<std::chrono::system_clock>& to, const Author& author) const = 0;
    };
}