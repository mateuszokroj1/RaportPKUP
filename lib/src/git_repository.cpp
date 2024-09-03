#include <execution>
#include <filesystem>

#include "Commit.hpp"
#include "LibGit.hpp"
#include "base.hpp"
#include <tbb/tbb.h>

#include "GitRepository.hpp"

namespace RaportPKUP
{
GitRepository::GitRepository(std::shared_ptr<LibGit_Repository> repo, const std::wstring& path)
	: _repository(repo), _path(path)
{
}

std::optional<Author> GitRepository::getDefaultAuthor() const
{
	if (!_repository)
		return {};

	return _repository->getAuthorFromConfig();
}

std::string GitRepository::getNameOfRemoteRepository() const
{
	auto list_of_remotes = _repository->enumerateRemotes();

	if (list_of_remotes.empty() || !list_of_remotes.front())
		return {};

	return list_of_remotes.front()->remoteNameOnServer();
}

short compareDate(const std::chrono::system_clock::time_point& value, const std::chrono::system_clock::time_point& from,
				  const std::chrono::system_clock::time_point& to)
{
	if (value < from)
		return -1;

	if (value > to)
		return 1;

	return 0;
}

std::list<Commit> GitRepository::getCommitsFromTimeRange(const std::chrono::system_clock::time_point& from,
														 const std::chrono::system_clock::time_point& to,
														 const Author& author, const std::stop_token& stop_token) const
{
	const auto branches = _repository->enumerateAllRemoteBranches();
	const auto repo_name = getNameOfRemoteRepository();

	if (stop_token.stop_requested())
		return {};

	auto mutex = std::make_unique<std::mutex>();
	std::list<Commit> list;

	std::for_each(std::execution::par, branches.cbegin(), branches.cend(),
				  [this, &stop_token, &author, &repo_name, &from, &to, &list, &mutex](const auto& branch)
				  {
					  auto walker = _repository->createWalker();
					  if (!walker)
						  return;

					  walker->setReference(*branch);
					  // Now commits are sorted descending by time

					  if (stop_token.stop_requested())
						  return;

					  while (const auto opt = walker->next())
					  {
						  if (const auto commit = *opt)
						  {
							  if (stop_token.stop_requested())
								  return;

							  Commit result;

							  const auto id = commit->id();
							  git_oid_tostr(reinterpret_cast<char*>(&result.id), 8, &id);

							  if (std::ranges::any_of(list, [&result](decltype(list)::const_reference previous_commit)
													  { return result.id == previous_commit.id; }))
								  continue;

							  result.datetime = commit->getTime();

							  const auto compare_result = compareDate(result.datetime, from, to);

							  if (compare_result < 0)
								  break;
							  else if (compare_result > 0)
								  continue;

							  result.author = commit->getAuthor();

							  if (author.email != result.author.email)
								  continue;

							  result.branch_name = branch->name();

							  result.repo_name = repo_name;
							  result.message = commit->getShortMessage();

							  std::lock_guard lock(*mutex);
							  list.push_back(std::move(result));
						  }
					  }
				  });

	if (stop_token.stop_requested())
		return {};

	return list;
}

std::future<bool> GitRepository::fetchFirstRemote(bool with_prune)
{
	return std::async(
		[this, with_prune]()
		{
			const auto remotes = _repository->enumerateRemotes();

			if (remotes.empty())
				return false;

			if (with_prune && !_repository->prune(*remotes.front()))
				return false;

			return _repository->fetch(*remotes.front());
		});
}
} // namespace RaportPKUP
