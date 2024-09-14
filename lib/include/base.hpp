#pragma once

#include <chrono>
#include <map>
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

template <typename T>
concept Comparable = requires(T a, T b) {
	a > b;
	a < b;
};

template <typename T>
concept Equatable = requires(T a, T b) {
	a == b;
	a != b;
};

template <typename T>
concept MovableMapKey = std::movable<T> && Equatable<T> && Comparable<T>;

template <std::movable DataType, MovableMapKey KeyType> class ISynchronizationContainerWrapper
{
  public:
	virtual ~ISynchronizationContainerWrapper() = default;

	virtual bool contains(const KeyType&, const std::stop_token& stop_token = {}) const = 0;
	virtual void emplace(KeyType&&, DataType&&, const std::stop_token& stop_token = {}) = 0;
};

template <std::movable DataType, MovableMapKey KeyType>
class SynchronizationContainerWrapper final : public ISynchronizationContainerWrapper<DataType, KeyType>
{
  public:
	SynchronizationContainerWrapper(std::map<KeyType, DataType>& container) : _container(container)
	{
	}

	bool contains(const KeyType& key, const std::stop_token& stop_token = {}) const
	{
		try
		{
			while (!_mutex.try_lock() && !stop_token.stop_requested())
				std::this_thread::yield();

			if (stop_token.stop_requested())
			{
				_mutex.unlock();
				return false;
			}

			const bool result = _container.contains(key);

			_mutex.unlock();

			return result;
		}
		catch (...)
		{
			_mutex.unlock();
			throw;
		}
	}

	void emplace(KeyType&& key, DataType&& data, const std::stop_token& stop_token = {}) override
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

			if (!_container.contains(key))
				_container.emplace(std::move(key), std::move(data));

			_mutex.unlock();
		}
		catch (...)
		{
			_mutex.unlock();
			throw;
		}
	}

  private:
	mutable std::mutex _mutex;
	std::map<KeyType, DataType>& _container;
};
} // namespace RaportPKUP
