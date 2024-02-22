#pragma once

#include <string>

#include "base.hpp"
#include "Author.hpp"
#include <raportgencore_export.h>

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