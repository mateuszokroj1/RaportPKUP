#pragma once

#include <chrono>
#include <filesystem>
#include <memory>
#include <optional>

#include "IRepository.hpp"

namespace RaportPKUP
{
class LibGit_Repository;
class GitRepositoryAccessor;

class GitRepository : public IRepository
{
  public:
	GitRepository(std::shared_ptr<LibGit_Repository>, const std::wstring& path);

	std::wstring path() const override
	{
		return _path;
	}

	std::optional<Author> getDefaultAuthor() const override;

	std::string getNameOfRemoteRepository() const override;

	std::future<bool> fetchFirstRemote(bool with_prune) override;

	std::future<std::list<Commit>> getCommitsFromTimeRange(
		const std::chrono::system_clock::time_point& from, const std::chrono::system_clock::time_point& to,
		const Author& author, std::optional<std::stop_token> stop_token = {}) const override;

  private:
	std::list<Commit> getCommitsFromTimeRangeImpl(const std::chrono::system_clock::time_point& from,
												  const std::chrono::system_clock::time_point& to, const Author& author,
												  std::optional<std::stop_token> stop_token) const;

	std::wstring _path;
	std::shared_ptr<LibGit_Repository> _repository;
};
} // namespace RaportPKUP
