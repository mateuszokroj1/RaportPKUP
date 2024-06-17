#pragma once

#include <QtQml/QQmlListProperty>

#include <include/IProcess.hpp>
#include <include/IRepositoryDetector.hpp>

#include "Application.hpp"
#include "MainViewItem.hpp"

namespace RaportPKUP::UI
{
class WindowController : public QObject
{
	Q_OBJECT

  public:
	WindowController(std::weak_ptr<Application> app);

	Q_PROPERTY(QQmlListProperty<MainViewItem> items READ items NOTIFY itemsChanged)

	// Q_PROPERTY(QString authorName READ authorName WRITE setAuthorName BINDABLE bindableAuthorName)
	// Q_PROPERTY(QString authorEmail READ authorEmail WRITE setAuthorEmail BINDABLE bindableAuthorEmail)

	// Q_PROPERTY(QQmlListProperty<>)

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
	// std::shared_ptr<IProcessFactory> _process_factory;
	// std::shared_ptr<IRepositoryDetector> _repository_detector;
};
} // namespace RaportPKUP::UI
