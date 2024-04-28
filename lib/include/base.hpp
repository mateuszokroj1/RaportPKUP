#pragma once

#include <chrono>
#include <type_traits>

namespace RaportGen
{
template <class DerivedType, class BaseType>
concept Derived = std::is_base_of_v<BaseType, DerivedType>;

template <class AbstractType>
concept Abstract = std::is_abstract_v<AbstractType>;

template <class Type>
concept Final = std::is_final_v<Type>;

using DateTime = std::chrono::time_point<std::chrono::system_clock>;
} // namespace RaportGen
