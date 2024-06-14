#pragma once

#include <chrono>
#include <type_traits>

namespace RaportPKUP
{
template <class DerivedType, class BaseType>
concept Derived = std::is_base_of_v<BaseType, DerivedType>;

template <class AbstractType>
concept Abstract = std::is_abstract_v<AbstractType>;

template <class Type>
concept Final = std::is_final_v<Type>;

using DateTime = std::chrono::utc_clock::time_point;

class ICastable
{
  public:
	virtual ~ICastable() = default;
};

template <typename ReturnedItemType> class IEnumerator
{
  public:
	virtual ~IEnumerator() = default;

	virtual std::optional<ReturnedItemType> next() = 0;
};

#define COPY_CONSTRUCTOR(Type) Type(const Type&)

class CanceledOperationException : public std::exception
{
  public:
	CanceledOperationException() : std::exception("Cancellation requested.")
	{
	}
};
} // namespace RaportPKUP
