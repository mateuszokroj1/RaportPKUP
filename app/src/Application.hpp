#pragma once

#include <memory>
#include <typeindex>
#include <typeinfo>

#include <QGuiApplication>

#include "ApplicationDefinition.hpp"
#include <mutex>

namespace RaportPKUP
{
class Application : private std::enable_shared_from_this<Application>
{
  public:
	Application() = default;
	Application(Application &&) = default;

	int run(int argc, char *argv[]);

	friend class ApplicationBuilder;

	template <class DependencyType>
	std::weak_ptr<DependencyType> get(const std::type_info &info = typeid(DependencyType))
	{
		decltype(_factories)::const_iterator iterator;
		constexpr std::type_index index(info);

		{
			std::unique_lock locker(_factory_mutex);

			if (_active_controllers.contains(index))
				return std::dynamic_pointer_cast<DependencyType>(_active_controllers[index]);

			iterator = std::find_if(_factories.cbegin(), _factories.cend(), [info](const FactoryInfo &pair)
									{ pair.interface == info || pair.controller == info; });

			if (iterator == _factories.cend())
				return {};
		}

		auto ptr_to_new_instance = iterator->factory(weak_from_this());

		{
			std::unique_lock locker(_factory_mutex);

			_active_controllers.emplace(index, ptr_to_new_instance);
		}

		return std::dynamic_pointer_cast<DependencyType>(ptr_to_new_instance);
	}

  private:
	Application(const Application &) = delete;

	bool _is_built = false;
	std::unique_ptr<QGuiApplication> _main_app;
	std::map<std::type_index, std::shared_ptr<void>> _active_controllers;
	std::vector<FactoryInfo> _factories;
	std::mutex _factory_mutex;
};

class ApplicationBuilder
{
  private:
	ApplicationBuilder() = delete;
	ApplicationBuilder(const ApplicationBuilder &) = delete;
	ApplicationBuilder(ApplicationBuilder &&) = delete;

  public:
	static void build(ApplicationDefinition &&, Application &);
};
} // namespace RaportPKUP
