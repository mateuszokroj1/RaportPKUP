#pragma once

#include <filesystem>
#include <future>
#include <memory>
#include <stop_token>

#include "base.hpp"

namespace RaportPKUP
{
class IRepository;

class IRepositoryAccessor : public ICastable
{
  public:
	virtual ~IRepositoryAccessor() = default;

	virtual std::future<std::shared_ptr<IRepository>> openRepository(
		const std::filesystem::path&, std::optional<std::stop_token> stop_token = {}) const = 0;
};
} // namespace RaportPKUP
