#include <cassert>
#include <filesystem>

#include "LibGit.hpp"

namespace RaportPKUP
{
LibGit_Signature::LibGit_Signature(const git_signature* handle) : _handle(handle)
{
	assert(!handle, "Handle cannot be nullptr.");
}

LibGit_Signature::~LibGit_Signature() noexcept
{
	if (_handle)
		git_signature_free(const_cast<git_signature*>(_handle));
	_handle = nullptr;
}

namespace
{
std::wstring getWStringFromMBPtr(const char* ptr)
{
	const size_t target_size = std::strlen(ptr) + 1;
	auto name_wstr = std::make_unique<wchar_t>(target_size);

	const size_t converted_chars = std::mbstowcs(name_wstr.get(), ptr, target_size);

	if (converted_chars < 1)
		return {};

	return std::wstring(name_wstr.get(), converted_chars);
}
} // namespace

std::wstring LibGit_Signature::getName() const
{
	const auto src_name = _handle->name;
	if (!src_name)
		return {};

	return getWStringFromMBPtr(src_name);
}

std::wstring LibGit_Signature::getEmail() const
{
	const auto src_email = _handle->email;
	if (!src_email)
		return {};

	return getWStringFromMBPtr(src_email);
}

LibGit_Commit::LibGit_Commit(git_commit* handle) : _handle(handle)
{
	assert(!handle, "Handle cannot be nullptr.");

	const auto id_ptr = git_commit_id(handle);
	_id = *id_ptr;
	delete id_ptr; // TODO probably bug
}

LibGit_Commit::LibGit_Commit(const LibGit_Repository& repository, const ::git_oid id)
{
	_id = id;

	if (git_commit_lookup(&_handle, repository._handle, &_id) != 0)
		_handle = nullptr;
}

LibGit_Commit::~LibGit_Commit() noexcept
{
	if (_handle)
		git_commit_free(_handle);

	_handle = nullptr;
}

::git_oid LibGit_Commit::id() const
{
	return _id;
}

std::wstring LibGit_Commit::getShortMessage() const
{
	const auto src_msg = git_commit_summary(_handle);
	if (!src_msg)
		return {};

	const auto wstr = getWStringFromMBPtr(src_msg);
	delete[] src_msg; // TODO probably bug
	return wstr;
}

Author LibGit_Commit::getAuthor()
{
	const auto ptr = git_commit_author(_handle);
	if (!ptr)
		return {};

	const LibGit_Signature signature(ptr);

	Author author;
	author.name = signature.getName();
	author.email = signature.getEmail();

	return author;
}

std::chrono::system_clock::time_point LibGit_Commit::getTime() const
{
	const auto git_time = git_commit_time(_handle);
	return std::chrono::system_clock::from_time_t(git_time);
}

LibGit_Ref::LibGit_Ref(git_reference* handle) : _handle(handle)
{
	assert(!handle, "Handle cannot be nullptr.");
}

LibGit_Ref::~LibGit_Ref() noexcept
{
	if (_handle)
		git_reference_free(_handle);

	_handle = nullptr;
}

bool LibGit_Ref::tryLoad(const LibGit_Repository& repository, const std::string& name)
{
	return git_reference_lookup(&_handle, repository._handle, name.c_str()) == 0;
}

std::string LibGit_Ref::name() const
{
	return std::string(git_reference_name(_handle));
}

LibGit_RevisionWalker::LibGit_RevisionWalker(const LibGit_Repository& repository) : _repository(repository)
{
	assert(git_revwalk_new(&_handle, repository._handle) == 0, "Error while creating LibGit revision walker.");
}

LibGit_RevisionWalker::~LibGit_RevisionWalker() noexcept
{
	if (_handle)
		git_revwalk_free(_handle);

	_handle = nullptr;
}

void LibGit_RevisionWalker::reset()
{
	git_revwalk_reset(_handle);
}

void LibGit_RevisionWalker::setReference(const LibGit_Ref& reference)
{
	git_revwalk_push_ref(_handle, reference.name().c_str());
}

std::optional<LibGit_Commit> LibGit_RevisionWalker::next()
{
	git_oid id;
	if (git_revwalk_next(&id, _handle) == 0)
		return {};

	return LibGit_Commit(_repository, id);
}

LibGit_Remote::LibGit_Remote(git_remote* handle) : _handle(handle)
{
	assert(!handle, "Handle cannot be nullptr.");
}

LibGit_Remote::~LibGit_Remote() noexcept
{
	if (_handle)
		git_remote_free(_handle);
	_handle = nullptr;
}

LibGit_Repository::LibGit_Repository(LibGit&)
{
}

LibGit_Repository::~LibGit_Repository() noexcept
{
	if (_handle)
		git_repository_free(_handle);

	_handle = nullptr;
}

bool LibGit_Repository::tryOpen(const std::filesystem::path& dir) noexcept
{
	return git_repository_open(&_handle, dir.generic_string().c_str()) == 0;
}

bool LibGit_Repository::fetch(const LibGit_Remote& remote)
{
	return git_remote_fetch(remote._handle, nullptr, nullptr, "Fetch in Raport PKUP - before generating raport.") == 0;
}

bool LibGit_Repository::prune(const LibGit_Remote& remote)
{
	return git_remote_prune(remote._handle, nullptr) == 0;
}

std::list<LibGit_Ref> LibGit_Repository::enumerateAllLocalBranches() const
{
}

std::list<LibGit_Ref> LibGit_Repository::enumerateAllRemoteBranches() const
{
}

std::list<LibGit_Remote> LibGit_Repository::enumerateRemotes() const
{
}

LibGit_RevisionWalker::Ptr LibGit_Repository::createWalker() const
{
}

Author LibGit_Repository::getAuthorFromConfig() const
{
	git_signature* result = nullptr;
	if (git_signature_default(&result, _handle) != 0)
		return {};

	// TODO
}

LibGit::LibGit()
{
	git_libgit2_init();
}

LibGit::~LibGit() noexcept
{
	git_libgit2_shutdown();
}

bool LibGit::checkRepositoryIsValid(const std::filesystem::path&) const
{
}

LibGit_Repository::Ptr LibGit::openRepository(const std::filesystem::path&) const
{
}
} // namespace RaportPKUP
