#include <filesystem>
#include <future>
#include <memory>
#include <optional>
#include <stop_token>

#include "GitRepository.hpp"
#include "LibGit.hpp"

#include "GitRepositoryAccessor.hpp"
#include "GitRepositoryDetector.hpp"

namespace RaportPKUP
{
GitRepositoryAccessor::GitRepositoryAccessor()
{
	git_libgit2_init();
}

GitRepositoryAccessor::~GitRepositoryAccessor()
{
	git_libgit2_shutdown();
}

namespace
{
IRepository::Ptr openRepositoryImpl(const std::filesystem::path& path, std::optional<std::stop_token> stop_token)
{
	if (stop_token && stop_token->stop_requested())
		throw CanceledOperationException();

	auto repo = std::make_shared<LibGit_Repository>();

	if (!repo->tryOpen(path))
		return {};

	return std::shared_ptr<GitRepository>(new GitRepository(repo, path.generic_wstring()));
}
} // namespace

std::future<std::shared_ptr<IRepository>> GitRepositoryAccessor::openRepository(
	const std::filesystem::path& path, std::optional<std::stop_token> stop_token) const
{
	return std::async(openRepositoryImpl, path, stop_token);
}

const IRepositoryAccessor& GitRepositoryDetector::getAccessor() const
{
	return getAccessorImpl();
}

std::future<std::optional<std::filesystem::path>> GitRepositoryDetector::detect(
	const std::filesystem::path& path, std::optional<std::stop_token> stop_token) const
{
	return std::async(
		[path, stop_token]() -> std::optional<std::filesystem::path>
		{
			if (stop_token && stop_token->stop_requested())
				throw CanceledOperationException();

			git_buf buf{0};
			if (git_repository_discover(&buf, path.generic_string().c_str(), NULL, nullptr) != 0)
				return {};

			if (buf.size < 2)
			{
				git_buf_free(&buf);
				return {};
			}

			std::string str(buf.ptr, buf.size);

			git_buf_dispose(&buf);

			return str;
		});
}
} // namespace RaportPKUP
