#include <signal.h>

#include <QQmlApplicationEngine>
#include <qqmlcontext.h>

#include "app_environment.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"

#include "Application.hpp"
#include "WindowController.h"

namespace
{
#ifndef RAPORTPKUP_NOSIGNAL
void SignalHandler(int signal)
{
	throw std::exception("Wykryto błąd uniemożliwiający dalszą pracę. Kod błedu: 1.");
}
#endif
} // namespace

namespace RaportPKUP
{

void ApplicationBuilder::build(ApplicationDefinition &&definition, Application &application)
{
	application._factories = std::move(definition._factories);

	application._is_built = true;
}

int Application::run(int argc, char *argv[])
{
	if (!_is_built)
		return -1;

	set_qt_environment();

	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	const QUrl url(u"qrc:/qt/qml/Main/main.qml"_qs);
	QObject::connect(
		&engine, &QQmlApplicationEngine::objectCreated, &app,
		[url](QObject *obj, const QUrl &objUrl)
		{
			if (!obj && url == objUrl)
				QCoreApplication::exit(-1);
		},
		Qt::QueuedConnection);

	engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
	engine.addImportPath(":/");

	auto context = engine.rootContext();
	if (!context)
		return -1;

	WindowController controller(weak_from_this());
	context->setContextProperty("controller", &controller);

	engine.load(url);

	if (engine.rootObjects().isEmpty())
	{
		return -1;
	}

#ifndef RAPORTPKUP_NOSIGNAL
	signal(SIGSEGV, SignalHandler);
#endif

	return app.exec();
}
} // namespace RaportPKUP
