#pragma once

#include "Commit.hpp"
#include "base.hpp"

#include <future>
#include <optional>
#include <stop_token>

namespace RaportPKUP
{
class Author;

class IRepository
{
  public:
	using Ptr = std::shared_ptr<IRepository>;

	virtual ~IRepository() = default;

	virtual std::wstring path() const = 0;

	virtual std::optional<Author> getDefaultAuthor() const = 0;

	virtual std::string getNameOfRemoteRepository() const = 0;

	virtual std::future<bool> fetchFirstRemote(bool with_prune) = 0;

	virtual void getCommitsFromTimeRange(const DateTime& from, const DateTime& to, const Author& author,
										 ISynchronizationContainerWrapper<Commit, Commit::Key>& container,
										 const std::stop_token& stop_token = {}) const = 0;
};
} // namespace RaportPKUP
