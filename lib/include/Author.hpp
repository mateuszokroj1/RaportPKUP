#pragma once

#include <string>

#include <raportgencore_export.h>

namespace RaportGen
{
    struct RAPORTGENCORE_EXPORT Author
    {
        std::wstring name;
        std::wstring email;
    };
}