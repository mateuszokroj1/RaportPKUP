#pragma once

#include <string>

#include "base.hpp"
#include "Author.hpp"

namespace RaportGen
{
    struct Commit
    {
        std::string message;
        DateTime datetime;
        std::string id;
        Author author;
    };
}