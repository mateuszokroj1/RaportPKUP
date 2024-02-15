#pragma once

#include <vector>
#include <memory>

#include "Commit.hpp"
#include "RaportGenCoreExport.h"

namespace RaportGen
{
    class RAPORTGENCORE_EXPORT IRepository
    {
    protected:
        IRepository() = default;
    
    public:
        IRepository(const IRepository&) = delete;
        IRepository(IRepository&&) = delete;

        virtual std::shared_ptr<std::vector<Commit>> getCommitsFromTimeRange(const time_t& from, const time_t& to) = 0;
    };
}