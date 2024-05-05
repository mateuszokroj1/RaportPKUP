#include "WindowController.hpp"
#include <include/IProcess.hpp>

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP
{
WindowController::WindowController(std::weak_ptr<Application> app) : _application(app)
{
	if (auto app_ptr = _application.lock())
	{
		auto weak = app_ptr->get<IProcessFactory>();
		if (!(_process_factory = weak.lock()))
			return;

		if (auto qml_engine = app_ptr->getQmlEngine())
			creatingSteps(qml_engine);
	}
}

void WindowController::creatingSteps(QQmlApplicationEngine *qml)
{
	auto item = new MainViewItem(this);
	{
		item->setName(u"Wprowadzanie danych"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/DataInputStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem *>(component.create());
		if (!view)
			return;

		item->setContent(view);

		_qlist.push_back(item);
	}

	{
		item = new MainViewItem(this);
		item->setName(u"Uzupełnianie godzin"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/DataInputStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem *>(component.create());
		if (!view)
			return;

		item->setContent(view);

		_qlist.push_back(item);
	}

	{
		item = new MainViewItem(this);
		item->setName(u"Generowanie raportu"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/ReportingStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem *>(component.create());
		if (!view)
			return;

		item->setContent(view);

		_qlist.push_back(item);
	}

	_items = QQmlListProperty<MainViewItem>(this, &_qlist);
	emit itemsChanged();
}
} // namespace RaportPKUP
