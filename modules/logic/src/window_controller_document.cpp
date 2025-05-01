#include "WindowController.hpp"

#include <QtGui/QTextCursor>

namespace RaportPKUP::UI
{
void WindowController::setDocumentInQml(QQuickTextDocument* text) const
{
	if (text)
		if (auto doc = document())
			text->setTextDocument(doc.get());
}

QSharedPointer<QTextDocument> WindowController::document() const
{
	const QSharedPointer<QTextDocument> document(new QTextDocument(const_cast<WindowController*>(this)));
	const QScopedPointer<QTextCursor> cursor(new QTextCursor(document.get()));

	document->setPageSize({595.0, 842.0});
	// document->setDocumentMargin(56.693); // pt PostScript

	return document;
}
} // namespace RaportPKUP::UI
