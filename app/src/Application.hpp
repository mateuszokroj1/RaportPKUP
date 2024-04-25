#pragma once

#include "ApplicationDefinition.hpp"

namespace RaportPKUP
{
int main(int argc, char *argv[]);

class Application
{
	int run();
};

class ApplicationBuilder
{
  public:
	ApplicationBuilder() = delete;
	ApplicationBuilder(const ApplicationBuilder &) = delete;
	ApplicationBuilder(ApplicationBuilder &&) = delete;

	static Application build();
};
} // namespace RaportPKUP
