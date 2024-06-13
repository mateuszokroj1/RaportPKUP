#pragma once

#include <future>
#include <list>
#include <stop_token>

namespace RaportPKUP
{
class Commit;
class Author;

class IRepository
{
  public:
	virtual ~IRepository() = default;

	virtual std::wstring path() const = 0;

	virtual std::optional<Author> getDefaultAuthor() const = 0;

	virtual std::future<std::list<Commit>> getCommitsFromTimeRange(
		const std::chrono::utc_clock::time_point& from, const std::chrono::utc_clock::time_point& to,
		const Author& author, std::optional<std::stop_token> stop_token = {}) const = 0;
};
} // namespace RaportPKUP
