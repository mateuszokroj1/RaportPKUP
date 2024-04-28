#pragma once

#include <algorithm>
#include <functional>
#include <memory>

#include <include/base.hpp>

using namespace RaportGen;

namespace RaportPKUP
{
class ApplicationBuilder;
class Application;

struct FactoryInfo
{
	using Factory = std::function<std::shared_ptr<void>(std::weak_ptr<Application>)>;

	template <typename InterfaceType, typename ControllerType> FactoryInfo(const Factory &factory) : factory(factory)
	{
		interface = typeid(InterfaceType);
		controller = typeid(ControllerType);
	}

	std::type_info interface;
	std::type_info controller;
	Factory factory;
};

template <class Type, class Base>
concept Controller = std::is_final_v<Type> && std::is_base_of_v<Type, Base>;

class ApplicationDefinition
{
  public:
	template <Abstract InterfaceType, Controller<InterfaceType> ControllerType>
	ApplicationDefinition &registerController()
	{
		if (std::any_of(_factories.cbegin(), _factories.cend(),
						[](const FactoryInfo &info) { return info.interface == typeid(InterfaceType); }))
		{
			throw std::exception("Currently one instance for interface is supported.");
		}

		if (std::any_of(_factories.cbegin(), _factories.cend(),
						[](const FactoryInfo &info) { return info.controller == typeid(ControllerType); }))
		{
			throw std::exception("Already registered.");
		}

		std::optional<FactoryInfo::Factory> factory;

		if (std::is_constructible_v<ControllerType, std::weak_ptr<Application>>)
		{
			factory = [](std::weak_ptr<Application> app)
			{
				ControllerType *new_instance = new ControllerType(app);
				return std::shared_ptr<void>(new_instance);
			};
		}

		if (std::is_default_constructible_v<ControllerType>)
		{
			factory = [](std::weak_ptr<Application>)
			{
				ControllerType *new_instance = new ControllerType{};
				return std::shared_ptr<void>(new_instance);
			};
		}

		if (!factory)
			throw std::exception("Not found any compatible constructors.");

		_factories.push_back(FactoryInfo<InterfaceType, ControllerType>(*factory));
	}

  private:
	std::vector<FactoryInfo> _factories;

	friend class ApplicationBuilder;
};
} // namespace RaportPKUP
