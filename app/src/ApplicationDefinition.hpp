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
	std::type_info interface;
	std::type_info controller;
	std::function<std::shared_ptr<void>(Application &)> factory;
};

class ApplicationDefinition
{
  public:
	template <class InterfaceType, Derived<InterfaceType> ControllerType> ApplicationDefinition &registerController()
	{

		// if(std::any_of(_factories.cbegin(), _factories.cend(), [](const FactoryInfo& info){ return info.interface ==
		// typeof(typename InterfaceType); }))
	}

  private:
	std::vector<FactoryInfo> _factories;

	friend class ApplicationBuilder;
};
} // namespace RaportPKUP
