#include <QtGui/QSurfaceFormat>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqmlcontext.h>

#include "Application.hpp"
#include "EmptyDeleter.hpp"
#include "WindowController.hpp"

#include <signal.h>

namespace
{
#ifndef RAPORTPKUP_NOSIGNAL
void SignalHandler(int signal)
{
	throw std::exception("Wykryto błąd uniemożliwiający dalszą pracę. Kod błedu: 1.");
}
#endif
} // namespace

namespace RaportPKUP::UI
{
void ApplicationBuilder::build(ApplicationDefinition&& definition, Application& application)
{
	application._factories = std::move(definition._factories);

	application._is_built = true;
}

Application::Application() : _ptr(this, EmptyDeleter<Application>{})
{
}

int Application::run(int argc, char* argv[])
{
	if (!_is_built || _is_running)
		return -1;

	{
		auto app = new QGuiApplication(argc, argv);

		_main_app.reset(app);

		QSurfaceFormat format;
		format.setSamples(8);
		QSurfaceFormat::setDefaultFormat(format);

		_qml = std::make_unique<QQmlApplicationEngine>();
		const QUrl url(u"qrc:/qt/qml/Main/main.qml"_qs);
		QObject::connect(
			_qml.get(), &QQmlApplicationEngine::objectCreated, app,
			[url](QObject* obj, const QUrl& objUrl)
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

		auto controller = new WindowController(_ptr);

		context->setContextProperty("controller", controller);

		_qml->load(url);

		if (_qml->rootObjects().isEmpty())
			return -1;

#ifndef RAPORTPKUP_NOSIGNAL
		signal(SIGSEGV, SignalHandler);
#endif

		controller->setParent(_qml->rootObjects().first());

		_is_running = true;
		const auto result = app->exec();
		_is_running = false;

		return result;
	}
}
} // namespace RaportPKUP::UI
