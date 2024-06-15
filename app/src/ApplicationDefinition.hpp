#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>
#include <typeindex>

#include <include/base.hpp>

namespace RaportPKUP::UI
{
class ApplicationBuilder;
class Application;

struct FactoryInfo
{
	using Factory = std::function<std::shared_ptr<ICastable>(std::weak_ptr<Application>)>;

	FactoryInfo(std::type_index interface, std::type_index controller, const Factory& factory)
		: factory(factory), interface(interface), controller(controller)
	{
	}

	std::type_index interface;
	std::type_index controller;
	Factory factory;
};

template <class Type, class Base>
concept Controller = std::is_final_v<Type> && std::is_base_of_v<Base, Type>;

class ApplicationDefinition
{
  public:
	template <Abstract InterfaceType, Controller<InterfaceType> ControllerType>
	ApplicationDefinition& registerController()
	{
		if (std::any_of(_factories.cbegin(), _factories.cend(),
						[](const FactoryInfo& info) { return info.interface == typeid(InterfaceType); }))
		{
			throw std::exception("Currently one instance for interface is supported.");
		}

		if (std::any_of(_factories.cbegin(), _factories.cend(),
						[](const FactoryInfo& info) { return info.controller == typeid(ControllerType); }))
		{
			throw std::exception("Already registered.");
		}

		std::optional<FactoryInfo::Factory> factory;

		if constexpr (std::is_constructible_v<ControllerType, std::weak_ptr<Application>>)
		{
			factory = [](std::weak_ptr<Application> app)
			{
				ControllerType* new_instance = new ControllerType(app);
				return std::shared_ptr<ICastable>(new_instance);
			};
		}

		if constexpr (std::is_default_constructible_v<ControllerType>)
		{
			factory = [](std::weak_ptr<Application>)
			{
				ControllerType* new_instance = new ControllerType;
				return std::shared_ptr<ICastable>(new_instance);
			};
		}

		if (!factory)
			throw std::exception("Not found any compatible constructors.");

		_factories.emplace_back(typeid(InterfaceType), typeid(ControllerType), *factory);

		return *this;
	}

  private:
	std::vector<FactoryInfo> _factories;

	friend class ApplicationBuilder;
};
} // namespace RaportPKUP::UI
