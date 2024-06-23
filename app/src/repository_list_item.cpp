#include "RepositoryListItem.hpp"

namespace RaportPKUP::UI
{
QString RepositoryListItem::path() const
{
	return QString::fromStdWString(_repository->path());
}

void RepositoryListItem::accept(IRepositoryVisitor& visitor)
{
	visitor.visit(*_repository);
}
} // namespace RaportPKUP::UI
