#pragma once

#include <QtCore/QObject>

#include <include/Author.hpp>
#include <include/IRepository.hpp>
#include <include/base.hpp>

namespace RaportPKUP::UI
{
using IRepositoryVisitor = IVisitor<const IRepository&>;
using IRepositoryVisitorAccept = IAcceptVisitor<const IRepository&>;

class RepositoryListItem : public QObject, public IRepositoryVisitorAccept
{
	Q_OBJECT

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

	void accept(IRepositoryVisitor&) override;

  private:
	const std::shared_ptr<IRepository> _repository;
};
} // namespace RaportPKUP::UI
