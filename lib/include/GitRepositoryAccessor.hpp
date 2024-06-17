#pragma once

#include "IRepositoryAccessor.hpp"

#include <raportpkup.core_export.h>

namespace RaportPKUP
{
class GitRepositoryDetector;
class IRepository;

class RAPORTPKUP_CORE_EXPORT GitRepositoryAccessor final : public IRepositoryAccessor
{
	friend class GitRepositoryDetector;

  public:
	GitRepositoryAccessor();

	std::future<std::shared_ptr<IRepository>> openRepository(
		const std::filesystem::path&, std::optional<std::stop_token> stop_token = {}) const override;

  private:
	std::shared_ptr<ICastable> _libgit;
};
} // namespace RaportPKUP
