#include "WindowController.hpp"

namespace RaportPKUP::UI
{
QSharedPointer<QTextDocument> WindowController::document()
{
	const QSharedPointer<QTextDocument> document(new QTextDocument(this));

	return document;
}
} // namespace RaportPKUP::UI
