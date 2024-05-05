#include "MainViewItem.hpp"

namespace RaportPKUP
{
QString MainViewItem::name() const
{
	return _name.value();
}
void MainViewItem::setName(QString new_name)
{
	_name.setValue(new_name);
}
QBindable<QString> MainViewItem::bindableName() const
{
	return &_name;
}

QQuickItem *MainViewItem::content() const
{
	return _content.value();
}
void MainViewItem::setContent(QQuickItem *new_value)
{
	_content.setValue(new_value);
}
QBindable<QQuickItem *> MainViewItem::bindableContent() const
{
	return &_content;
}

bool MainViewItem::isEnabled() const
{
	return _is_enabled.value();
}
void MainViewItem::enable(bool new_value)
{
	_is_enabled.setValue(new_value);
}
QBindable<bool> MainViewItem::bindableIsEnabled() const
{
	return &_is_enabled;
}
} // namespace RaportPKUP
