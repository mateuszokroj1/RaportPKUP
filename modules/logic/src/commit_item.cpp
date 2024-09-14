#include "CommitItem.hpp"

namespace RaportPKUP::UI
{
CommitItem::CommitItem(const Commit& commit, QObject* parent) : QObject(parent), _commit(commit)
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
	return QDateTime::fromStdTimePoint(_commit.datetime);
}

QString CommitItem::repositoryName() const
{
	return QString::fromStdString(_commit.repo_name);
}
} // namespace RaportPKUP::UI
