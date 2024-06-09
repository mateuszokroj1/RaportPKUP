#pragma once

#include <QtQml/QQmlListProperty>

#include "Application.hpp"
#include "MainViewItem.hpp"
#include "include/IProcess.hpp"

namespace RaportPKUP::UI
{
class WindowController : public QObject
{
	Q_OBJECT

  public:
	WindowController(std::weak_ptr<Application> app);

	Q_PROPERTY(QQmlListProperty<MainViewItem> items READ items NOTIFY itemsChanged)

	QQmlListProperty<MainViewItem> items() const
	{
		return _items;
	}

  signals:
	void itemsChanged();

  private:
	void creatingSteps(QQmlApplicationEngine* qml);

	std::weak_ptr<Application> _application;
	QList<MainViewItem*> _qlist;
	QQmlListProperty<MainViewItem> _items;
	std::shared_ptr<IProcessFactory> _process_factory;
};
} // namespace RaportPKUP::UI
