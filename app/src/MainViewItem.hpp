#pragma once

#include "qqmlintegration.h"
#include <QtCore/QBindable>
#include <QtCore/QObject>
#include <QtQuick/QQuickItem>
#include <qstringliteral.h>

namespace RaportPKUP::UI
{
class MainViewItem : public QObject
{
  public:
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged BINDABLE bindableName DESIGNABLE true REQUIRED)
	Q_PROPERTY(QQuickItem *content READ content WRITE setContent NOTIFY contentChanged BINDABLE bindableContent
				   DESIGNABLE true)
	Q_PROPERTY(
		bool isEnabled READ isEnabled WRITE enable NOTIFY isEnabledChanged BINDABLE bindableIsEnabled DESIGNABLE true)

  public:
	MainViewItem(QObject *parent = nullptr) : QObject(parent)
	{
	}

	QString name() const;
	void setName(QString new_name);
	QBindable<QString> bindableName() const;

	QQuickItem *content() const;
	void setContent(QQuickItem *new_value);
	QBindable<QQuickItem *> bindableContent() const;

	bool isEnabled() const;
	void enable(bool new_value);
	QBindable<bool> bindableIsEnabled() const;

  signals:
	void nameChanged();
	void contentChanged();
	void isEnabledChanged();

  private:
	Q_OBJECT_BINDABLE_PROPERTY(MainViewItem, QString, _name, &MainViewItem::nameChanged)
	Q_OBJECT_BINDABLE_PROPERTY(MainViewItem, QQuickItem *, _content, &MainViewItem::contentChanged)
	Q_OBJECT_BINDABLE_PROPERTY(MainViewItem, bool, _is_enabled, &MainViewItem::isEnabledChanged)
};
} // namespace RaportPKUP::UI
