#pragma once

#include <chrono>
#include <type_traits>

namespace RaportPKUP
{
template <class AbstractType>
concept Abstract = std::is_abstract_v<AbstractType>;

template <class Type>
concept Final = std::is_final_v<Type>;

using DateTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

template <typename Type> using Ptr = std::shared_ptr<Type>;

class ICastable
{
  public:
	virtual ~ICastable() = default;
};

template <typename... ArgsTypes> class IVisitor
{
  public:
	virtual ~IVisitor() = default;

	virtual void visit(ArgsTypes...) = 0;
};

template <typename... ArgsTypes> class IAcceptVisitor
{
  public:
	virtual ~IAcceptVisitor() = default;

	virtual void accept(IVisitor<ArgsTypes...>&) = 0;
};

template <typename ContainerType, typename T>
concept SharedPointer =
	std::is_same_v<ContainerType, std::optional<T>> || std::is_same_v<ContainerType, std::shared_ptr<T>>;

template <typename ValueType, SharedPointer<ValueType> ReturnType = std::shared_ptr<ValueType>> class IEnumerator
{
  public:
	virtual ~IEnumerator() = default;

	virtual ReturnType next() = 0;
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
