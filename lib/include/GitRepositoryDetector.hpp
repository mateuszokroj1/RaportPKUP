#pragma once

#include "IRepositoryDetector.hpp"

#include <raportpkup.core_export.h>

namespace RaportPKUP
{
class GitRepositoryAccessor;

class RAPORTPKUP_CORE_EXPORT GitRepositoryDetector : public IRepositoryDetector
{
  public:
	const IRepositoryAccessor& getAccessor() const override;

	std::future<std::optional<std::filesystem::path>> detect(
		const std::filesystem::path&, std::optional<std::stop_token> stop_token = {}) const override;

  private:
	virtual const GitRepositoryAccessor& getAccessorImpl() const = 0;
};
} // namespace RaportPKUP
