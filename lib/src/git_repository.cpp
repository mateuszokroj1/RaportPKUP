#include <filesystem>

#include "Commit.hpp"
#include "LibGit.hpp"
#include "base.hpp"
#include <tbb/tbb.h>

#include "GitRepository.hpp"

namespace RaportPKUP
{
GitRepository::GitRepository(const LibGit& libgit, std::shared_ptr<LibGit_Repository> repo, const std::wstring& path)
	: _libgit(libgit), _repository(repo), _path(path)
{
}

std::optional<Author> GitRepository::getDefaultAuthor() const
{
	if (!_repository)
		return {};

	return _repository->getAuthorFromConfig();
}

std::future<std::list<Commit>> GitRepository::getCommitsFromTimeRange(const std::chrono::system_clock::time_point& from,
																	  const std::chrono::system_clock::time_point& to,
																	  const Author& author,
																	  std::optional<std::stop_token> stop_token) const
{
	return std::async(&GitRepository::getCommitsFromTimeRangeImpl, this, from, to, author, stop_token);
}

std::string GitRepository::getNameOfRemoteRepository() const
{
	auto list_of_remotes = _repository->enumerateRemotes();

	if (list_of_remotes.empty() || !list_of_remotes.front())
		return {};

	return list_of_remotes.front()->remoteNameOnServer();
}

namespace
{
template <typename ValueType> bool _isInRange(const ValueType& value, const ValueType& from, const ValueType& to)
{
	return value >= from && value <= to;
}
} // namespace

bool isInDateRange(const std::chrono::system_clock::time_point& value,
				   const std::chrono::system_clock::time_point& from, const std::chrono::system_clock::time_point& to)
{
	const std::chrono::year_month_day value_ymd(std::chrono::floor<std::chrono::days>(value));
	const std::chrono::year_month_day from_ymd(std::chrono::floor<std::chrono::days>(from));
	const std::chrono::year_month_day to_ymd(std::chrono::floor<std::chrono::days>(to));

	return _isInRange(value_ymd.year(), from_ymd.year(), to_ymd.year()) &&
		   _isInRange(value_ymd.month(), from_ymd.month(), to_ymd.month()) &&
		   _isInRange(value_ymd.day(), from_ymd.day(), to_ymd.day());
}

std::list<Commit> GitRepository::getCommitsFromTimeRangeImpl(const std::chrono::system_clock::time_point& from,
															 const std::chrono::system_clock::time_point& to,
															 const Author& author,
															 std::optional<std::stop_token> stop_token) const
{
	const auto branches = _repository->enumerateAllRemoteBranches();
	const auto repo_name = getNameOfRemoteRepository();

	if (stop_token && stop_token->stop_requested())
		throw CanceledOperationException();

	std::unique_ptr<std::mutex> mutex;
	std::list<Commit> list;

	for (auto branch : branches)
	//	tbb::parallel_for_each(branches.cbegin(), branches.cend(),
	//					   [this, &repo_name, stop_token, author, from, to, &mutex, &list](const LibGit_Ref::Ptr ref)
	{
		auto walker = _repository->createWalker();
		if (!walker)
			break;

		walker->setReference(*branch);

		//   if (stop_token && stop_token->stop_requested())
		//   {
		//	   tbb::task::current_context()->cancel_group_execution();
		//	   return;
		//   }

		while (const auto opt = walker->next())
		{
			if (const auto commit = *opt)
			{
				//	   if (stop_token && stop_token->stop_requested())
				//		   {
				//			   tbb::task::current_context()->cancel_group_execution();
				//			   return;
				//		   }

				Commit result;
				result.branch_name = branch->name();
				result.datetime = commit->getTime();
				result.repo_name = repo_name;

				// if (!isInDateRange(result.datetime, from, to))
				//    return;

				// result.author = commit->getAuthor();

				// if (author.email != result.author.email)
				//    continue;

				result.message = commit->getShortMessage();

				const auto id = commit->id();
				git_oid_tostr(reinterpret_cast<char*>(&result.id), 8, &id);

				// std::unique_lock locker(*mutex);
				list.push_back(std::move(result));
			}
		}
	} //);

	if (stop_token && stop_token->stop_requested())
		throw CanceledOperationException();

	return list;
}
} // namespace RaportPKUP
