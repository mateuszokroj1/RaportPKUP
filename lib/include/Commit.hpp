#pragma once

#include <string>

#include "Author.hpp"
#include "RaportGenCoreExport.h"

namespace RaportGen
{
    struct RAPORTGENCORE_EXPORT Commit
    {
        std::wstring message;
        time_t datetime;
        std::string id;
        Author author;
    };
}