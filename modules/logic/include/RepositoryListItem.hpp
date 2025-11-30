#pragma once

#include "qqmlintegration.h"
#include <QtCore/QObject>

#include <include/Author.hpp>
#include <include/IRepository.hpp>
#include <include/base.hpp>

namespace RaportPKUP::UI
{
class RepositoryListItem : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_UNCREATABLE("From controller")

  public:
	RepositoryListItem(std::shared_ptr<IRepository>&& repository, QObject* parent)
		: QObject(parent), _repository(repository)
	{
	}

	Q_PROPERTY(QString path READ path STORED false CONSTANT)

	QString path() const;
	std::optional<Author> getDefaultAuthor() const
	{
		return _repository->getDefaultAuthor();
	}

	const std::shared_ptr<const IRepository> getRepository() const
	{
		return _repository;
	}

  private:
	const std::shared_ptr<IRepository> _repository;
};
} // namespace RaportPKUP::UI
