#pragma once

#include <filesystem>
#include <future>

#include <raportgencore_export.h>

#include "IRepository.hpp"

namespace RaportGen
{
class RAPORTGENCORE_EXPORT GitRepository : public IRepository
{
  public:
	explicit GitRepository(const std::filesystem::path &);
	~GitRepository() override;

  private:
	GitRepository() = default;

	const std::filesystem::path _dir;

	static std::vector<std::string> getLog(const std::wstring &path, const std::string &from, const std::string &to,
										   const std::wstring &author);

  public:
	static bool checkIsValidPath(const std::filesystem::path &);
	static Author getSystemConfigAuthor();
	std::shared_ptr<std::vector<Commit>> getCommitsFromTimeRange(
		const std::chrono::time_point<std::chrono::system_clock> &from,
		const std::chrono::time_point<std::chrono::system_clock> &to, const Author &author) const override;
};
} // namespace RaportGen
