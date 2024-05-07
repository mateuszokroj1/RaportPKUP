#include <signal.h>

#include <QQmlApplicationEngine>
#include <qqmlcontext.h>

#include "app_environment.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"

#include "Application.hpp"
#include "WindowController.hpp"

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

Application::Application() : _ptr(this, EmptyDeleter<Application>{})
{
}

int Application::run(int argc, char *argv[])
{
	if (!_is_built || _is_running)
		return -1;

	{
		SessionSetter<bool, true, false> locker(&_is_running);

		set_qt_environment();

		auto app = new QGuiApplication(argc, argv);
		_main_app.reset(app);

		_qml = std::make_unique<QQmlApplicationEngine>();
		const QUrl url(u"qrc:/qt/qml/Main/main.qml"_qs);
		QObject::connect(
			_qml.get(), &QQmlApplicationEngine::objectCreated, app,
			[url](QObject *obj, const QUrl &objUrl)
			{
				if (!obj && url == objUrl)
					QCoreApplication::exit(-1);
			},
			Qt::QueuedConnection);

		_qml->addImportPath(QCoreApplication::applicationDirPath() + "/qml");
		_qml->addImportPath(":/");

		auto context = _qml->rootContext();
		if (!context)
			return -1;

		WindowController controller(_ptr);
		context->setContextProperty("controller", &controller);

		_qml->load(url);

		if (_qml->rootObjects().isEmpty())
			return -1;

#ifndef RAPORTPKUP_NOSIGNAL
		signal(SIGSEGV, SignalHandler);
#endif

		controller.setParent(_qml->rootObjects().first());

		return app->exec();
	}
}
} // namespace RaportPKUP
