#include "RepositoryListItem.hpp"

namespace RaportPKUP::UI
{
QString RepositoryListItem::path() const
{
	return QString::fromStdWString(_repository->path());
}
} // namespace RaportPKUP::UI
