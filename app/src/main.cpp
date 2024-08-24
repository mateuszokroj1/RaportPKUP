#include "app_environment.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"

#include "Application.hpp"
#include "Process.hpp"
#include "include/IProcess.hpp"

#include "GitAccessImpl.hpp"
#include <include/GitRepositoryAccessor.hpp>

namespace RaportPKUP
{
GitRepositoryDetectorDI::GitRepositoryDetectorDI(std::weak_ptr<UI::Application> app)
{
	auto application = app.lock();
	if (!application)
		throw std::exception("");

	if (auto ptr = application->get<GitRepositoryAccessor>().lock())
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

	set_qt_environment();
	std::setlocale(LC_ALL, "pl_PL.UTF-8");
	app.run(argc, argv);
}
