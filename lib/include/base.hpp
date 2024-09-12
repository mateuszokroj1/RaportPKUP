#pragma once

#include <chrono>
#include <mutex>
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

template <typename ContainerType, typename DataType>
concept PushBackAvailable = requires(DataType data, ContainerType container) { container.push_back(std::move(data)); };

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

template <std::movable DataType> class ISynchronizationContainerWrapper
{
  public:
	virtual ~ISynchronizationContainerWrapper() = default;

	virtual void push_back(DataType&& data, const std::stop_token& stop_token = {}) = 0;
};

template <std::movable DataType, PushBackAvailable<DataType> ContainerType>
class SynchronizationContainerWrapper final : public ISynchronizationContainerWrapper<DataType>
{
  public:
	SynchronizationContainerWrapper(ContainerType& container) : _container(container)
	{
	}

	void push_back(DataType&& data, const std::stop_token& stop_token = {}) override
	{
		try
		{
			while (!_mutex.try_lock() && !stop_token.stop_requested())
				std::this_thread::yield();

			if (stop_token.stop_requested())
			{
				_mutex.unlock();
				return;
			}

			_container.push_back(data);

			_mutex.unlock();
		}
		catch (...)
		{
			_mutex.unlock();
			throw;
		}
	}

  private:
	std::mutex _mutex;
	ContainerType& _container;
};
} // namespace RaportPKUP
