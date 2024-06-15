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
GitRepositoryAccessor::GitRepositoryAccessor() : _libgit(std::make_unique<LibGit>())
{
}

namespace
{
IRepository::Ptr openRepositoryImpl(const LibGit& libgit, const std::filesystem::path& path,
									std::optional<std::stop_token> stop_token)
{
	if (stop_token && stop_token->stop_requested())
		throw CanceledOperationException();

	auto ptr = libgit.openRepository(path);
	if (!ptr)
		return {};

	return std::shared_ptr<GitRepository>(new GitRepository(libgit, ptr, path.generic_wstring()));
}
} // namespace

std::future<std::shared_ptr<IRepository>> GitRepositoryAccessor::openRepository(
	const std::filesystem::path& path, std::optional<std::stop_token> stop_token) const
{
	return std::async(openRepositoryImpl, *std::dynamic_pointer_cast<LibGit>(_libgit), path, stop_token);
}

const IRepositoryAccessor& GitRepositoryDetector::getAccessor() const
{
	return getAccessorImpl();
}

std::future<std::optional<std::filesystem::path>> GitRepositoryDetector::detect(
	const std::filesystem::path& path, std::optional<std::stop_token> stop_token) const
{
	const auto& libgit = *std::dynamic_pointer_cast<LibGit>(getAccessorImpl()._libgit);
	return std::async(
		[libgit, path, stop_token]() -> std::optional<std::filesystem::path>
		{
			if (stop_token && stop_token->stop_requested())
				throw CanceledOperationException();

			return libgit.detectRepositoryRootPath(path);
		});
}
} // namespace RaportPKUP
