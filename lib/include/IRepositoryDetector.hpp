#pragma once

#include <filesystem>
#include <future>
#include <optional>

namespace RaportPKUP
{
class IRepositoryAccessor;

class IRepositoryDetector
{
  public:
	virtual ~IRepositoryDetector() = default;

	virtual const IRepositoryAccessor& getAccessor() const = 0;
	virtual std::future<std::optional<std::filesystem::path>> detect(
		const std::filesystem::path&, std::optional<std::stop_token> stop_token = {}) const = 0;
};
} // namespace RaportPKUP
