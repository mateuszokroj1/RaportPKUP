#include "CommitItem.hpp"

namespace RaportPKUP::UI
{
CommitItem::CommitItem(const Commit& commit, QObject* parent)
	: QObject(parent), _commit(commit), _date(QDateTime::fromStdTimePoint(_commit.datetime))
{
}

QString CommitItem::id() const
{
	return QString::fromStdString(_commit.id);
}

QString CommitItem::message() const
{
	return QString::fromStdWString(_commit.message);
}

QDateTime CommitItem::time() const
{
	return _date;
}

QString CommitItem::timeString() const
{
	return _date.toString("dd-MM-yyyy");
}

QString CommitItem::repositoryName() const
{
	return QString::fromStdString(_commit.repo_name);
}
} // namespace RaportPKUP::UI
