#pragma once

#include <string>

#include "RaportGenCoreExport.h"

namespace RaportGen
{
    struct RAPORTGENCORE_EXPORT Author
    {
        std::wstring name;
        std::wstring email;
    };
}