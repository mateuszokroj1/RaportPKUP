#include "Application.hpp"
#include "Process.hpp"
#include "include/IProcess.hpp"

#include "GitAccessImpl.hpp"
#include <include/GitRepositoryAccessor.hpp>

namespace RaportPKUP
{
GitRepositoryDetectorDI::GitRepositoryDetectorDI(UI::Application& app)
{
	if (auto ptr = app.get<GitRepositoryAccessor>().lock())
	{
		_accessor = ptr;
	}
	else
		throw std::exception("Cannot get git accessor.");
}
} // namespace RaportPKUP

using namespace RaportPKUP;
using namespace RaportPKUP::UI;

#ifdef WIN32

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	auto argc = __argc;
	auto argv = __argv;
#else

int main(int argc, char* argv[])
{
#endif

	ApplicationDefinition definition;

	definition.registerController<IProcessFactory, ProcessFactory>()
		.registerController<IRepositoryAccessor, GitRepositoryAccessor>()
		.registerController<IRepositoryDetector, GitRepositoryDetectorDI>();

	Application app;
	ApplicationBuilder::build(std::move(definition), app);

	app.run(argc, argv);
}
