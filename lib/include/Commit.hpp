#pragma once

#include <string>

#include "Author.hpp"
#include "RaportGenCoreExport.h"

namespace RaportGen
{
    struct RAPORTGENCORE_EXPORT Commit
    {
        std::string message;
        DateTime datetime;
        std::string id;
        Author author;
    };
}