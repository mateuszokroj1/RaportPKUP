#pragma once

#include <chrono>
#include <type_traits>

namespace RaportGen
{
template <class BaseType, class DerivedType>
concept Derived = std::is_base_of_v<BaseType, DerivedType>;

using DateTime = std::chrono::time_point<std::chrono::system_clock>;
} // namespace RaportGen
