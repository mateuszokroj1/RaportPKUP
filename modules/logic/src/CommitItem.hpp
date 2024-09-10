#pragma once

#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtQml/qqmlregistration.h>

#include <include/Commit.hpp>

namespace RaportPKUP::UI
{
class CommitItem : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_UNCREATABLE("Read only")

  public:
	CommitItem(std::unique_ptr<Commit>&&, QObject*);

	Q_PROPERTY(QString id READ id STORED false CONSTANT)
	Q_PROPERTY(QString message READ message STORED false CONSTANT)
	Q_PROPERTY(QDateTime time READ time STORED false CONSTANT)
	Q_PROPERTY(QString repositoryName READ repositoryName STORED false CONSTANT)
	Q_PROPERTY(ushort duration MEMBER duration NOTIFY durationChanged)

	QString id() const;
	QString message() const;
	QDateTime time() const;
	QString repositoryName() const;

	ushort duration = 0;

	Q_SIGNAL void durationChanged();

  private:
	std::unique_ptr<Commit> _commit;
};
} // namespace RaportPKUP::UI
