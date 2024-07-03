#pragma once

#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtQml/qqmlregistration.h>

namespace RaportPKUP::UI
{
class Preset : public QObject
{
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)

	Q_PROPERTY(QString authorName MEMBER authorName NOTIFY authorNameChanged)
	Q_PROPERTY(QString authorEmail MEMBER authorEmail NOTIFY authorEmailChanged)

	Q_PROPERTY(QString city MEMBER city NOTIFY cityChanged)

	Q_PROPERTY(QVector<QString> repositories MEMBER repositories NOTIFY repositoriesChanged)

  public:
	Preset(QObject* parent = nullptr) : QObject(parent)
	{
	}
	Preset(const Preset&) = delete;

	QString name;
	QString authorName;
	QString authorEmail;
	QString city;
	QVector<QString> repositories;

  signals:
	void nameChanged();
	void authorNameChanged();
	void authorEmailChanged();
	void cityChanged();
	void repositoriesChanged();
};
} // namespace RaportPKUP::UI
