#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <vector>

#include "Commit.hpp"

namespace RaportPKUP
{
class IRepository
{
  protected:
	IRepository() = default;

  public:
	IRepository(const IRepository&) = delete;
	IRepository(IRepository&&) = delete;
	virtual ~IRepository()
	{
	}

	virtual std::shared_ptr<std::vector<Commit>> getCommitsFromTimeRange(
		const std::chrono::time_point<std::chrono::system_clock>& from,
		const std::chrono::time_point<std::chrono::system_clock>& to, const Author& author) const = 0;
};
} // namespace RaportPKUP
