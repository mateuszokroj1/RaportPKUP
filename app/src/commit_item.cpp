#include "CommitItem.hpp"

namespace RaportPKUP::UI
{
CommitItem::CommitItem(const Commit& commit, QObject* parent) : QObject(parent), _commit(commit)
{
}

QString CommitItem::id() const
{
	QString str;
	str.append(_commit.id[0])
		.append(_commit.id[1])
		.append(_commit.id[2])
		.append(_commit.id[3])
		.append(_commit.id[4])
		.append(_commit.id[5])
		.append(_commit.id[6]);

	return str;
}

QString CommitItem::message() const
{
	return QString::fromStdWString(_commit.message);
}

QDateTime CommitItem::time() const
{
	return QDateTime::fromStdTimePoint(_commit.datetime);
}
} // namespace RaportPKUP::UI
