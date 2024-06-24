#pragma once

#include <memory>
#include <mutex>
#include <typeindex>

#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "ApplicationDefinition.hpp"

namespace RaportPKUP::UI
{
class Application final
{
  public:
	friend class ApplicationBuilder;

	Application();

	int run(int argc, char* argv[]);

	QPointer<QQmlApplicationEngine> getQmlEngine() const
	{
		return {_qml.get()};
	}

	QPointer<QGuiApplication> getQApplication() const
	{
		return {_main_app.get()};
	}

	template <class DependencyType> std::weak_ptr<DependencyType> get()
	{
		const std::type_index search_index = typeid(DependencyType);

		auto iterator = _factories.end();

		{
			std::unique_lock locker(_factory_mutex);

			if (_active_controllers.contains(search_index))
				return std::dynamic_pointer_cast<DependencyType>(_active_controllers[search_index]);

			iterator = std::find_if(_factories.begin(), _factories.end(), [&search_index](const FactoryInfo& pair)
									{ return pair.inter == search_index || pair.controller == search_index; });

			if (iterator == _factories.cend())
				return {};
		}

		const auto controller_index = iterator->controller;

		if (_active_controllers.contains(controller_index))
			return std::dynamic_pointer_cast<DependencyType>(_active_controllers[controller_index]);

		auto ptr_to_new_instance = iterator->factory(_ptr);

		{
			std::unique_lock locker(_factory_mutex);

			_active_controllers.emplace(controller_index, ptr_to_new_instance);
		}

		return std::dynamic_pointer_cast<DependencyType>(ptr_to_new_instance);
	}

  private:
	Application(const Application&) = delete;
	Application(Application&&) = delete;

	bool _is_built = false;
	bool _is_running = false;
	std::unique_ptr<QGuiApplication> _main_app;
	std::unique_ptr<QQmlApplicationEngine> _qml;
	std::map<std::type_index, std::shared_ptr<ICastable>> _active_controllers;
	std::vector<FactoryInfo> _factories;
	std::mutex _factory_mutex;
	std::shared_ptr<Application> _ptr;
};

class ApplicationBuilder
{
  private:
	ApplicationBuilder() = delete;
	ApplicationBuilder(const ApplicationBuilder&) = delete;
	ApplicationBuilder(ApplicationBuilder&&) = delete;

  public:
	static void build(ApplicationDefinition&&, Application&);
};
} // namespace RaportPKUP::UI
