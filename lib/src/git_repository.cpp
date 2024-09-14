#include "GitRepository.hpp"

#include "Commit.hpp"
#include "LibGit.hpp"
#include "base.hpp"

#include <execution>
#include <filesystem>

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

short compareDate(const DateTime& value, const DateTime& from, const DateTime& to)
{
	if (value < from)
		return -1;

	if (value > to)
		return 1;

	return 0;
}

void GitRepository::getCommitsFromTimeRange(const DateTime& from, const DateTime& to, const Author& author,
											ISynchronizationContainerWrapper<Commit, Commit::Key>& container,
											const std::stop_token& stop_token) const
{
	const auto branches = _repository->enumerateAllRemoteBranches();
	const auto repo_name = getNameOfRemoteRepository();

	if (stop_token.stop_requested())
		return;

	std::for_each(std::execution::par, branches.cbegin(), branches.cend(),
				  [this, &stop_token, &author, &repo_name, &from, &to, &container](const LibGit_Ref::Ptr& branch)
				  {
					  auto walker = _repository->createWalker();
					  if (!walker)
						  return;

					  walker->setReference(*branch);
					  // Now commits are sorted descending by time

					  if (stop_token.stop_requested())
						  return;

					  while (const auto commit = walker->next())
					  {
						  if (stop_token.stop_requested())
							  return;

						  Commit result;

						  const auto id = commit->id();
						  git_oid_tostr(result.id.value.data(), 8, &id);

						  result.datetime = commit->getTime();

						  const auto compare_result = compareDate(result.datetime, from, to);

						  if (compare_result < 0)
							  break;
						  if (compare_result > 0)
							  continue;

						  Commit::Key key{result.id, result.datetime};

						  if (container.contains(key))
							  continue;

						  result.author = commit->getAuthor();

						  if (author.email != result.author.email)
							  continue;

						  result.branch_name = branch->name();

						  result.repo_name = repo_name;
						  result.message = commit->getShortMessage();

						  container.emplace(std::move(key), std::move(result), stop_token);
					  }
				  });
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
