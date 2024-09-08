#pragma once

#include "base.hpp"

#include <future>
#include <list>
#include <optional>
#include <stop_token>

namespace RaportPKUP
{
class Commit;
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

	virtual std::list<Commit> getCommitsFromTimeRange(const DateTime& from, const DateTime& to, const Author& author,
													  const std::stop_token& stop_token = {}) const = 0;
};
} // namespace RaportPKUP
