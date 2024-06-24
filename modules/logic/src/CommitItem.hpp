#pragma once

#include <QtCore/QDateTime>
#include <QtCore/QObject>

#include <include/Commit.hpp>

namespace RaportPKUP::UI
{
class CommitItem : public QObject
{
	Q_OBJECT

  public:
	CommitItem(const Commit&, QObject*);

	Q_PROPERTY(QString id READ id STORED false)
	Q_PROPERTY(QString message READ message STORED false)
	Q_PROPERTY(QDateTime time READ time STORED false)
	Q_PROPERTY(ushort duration MEMBER duration NOTIFY durationChanged)

	QString id() const;
	QString message() const;
	QDateTime time() const;

	ushort duration;

	Q_SIGNAL void durationChanged();

  private:
	const Commit _commit;
};
} // namespace RaportPKUP::UI
