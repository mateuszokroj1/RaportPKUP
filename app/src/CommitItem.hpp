#pragma once

#include <QtCore/QObject>

#include <include/Commit.hpp>

namespace RaportPKUP::UI
{
class CommitItem : public QObject
{
	Q_OBJECT

  public:
	CommitItem(const Commit&, QObject*);

	// Q_PROPERTY()
};
} // namespace RaportPKUP::UI
