#pragma once

#include <filesystem>

#include <raportpkup.core_export.h>

#include "IRepository.hpp"

namespace RaportPKUP
{
class RAPORTPKUP_CORE_EXPORT GitRepository : public IRepository
{
  public:
	explicit GitRepository(const std::filesystem::path&);
	~GitRepository() override;

  private:
	GitRepository() = default;

	const std::filesystem::path _dir;

	static std::vector<std::string> getLog(const std::wstring& path, const std::string& from, const std::string& to,
										   const std::wstring& author);

  public:
	std::optional<Author> getDefaultAuthor() const override;
	std::vector<Commit> getCommitsFromTimeRange(const std::chrono::time_point<std::chrono::system_clock>& from,
												const std::chrono::time_point<std::chrono::system_clock>& to,
												const Author& author) const override;
};
} // namespace RaportPKUP
