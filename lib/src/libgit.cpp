#include <cassert>
#include <filesystem>

#include <LibGit.hpp>

namespace RaportPKUP
{
LibGit_Signature::LibGit_Signature(const git_signature* handle) : _handle(handle)
{
	if (!handle)
		throw std::invalid_argument("Handle cannot be nullptr.");
}

LibGit_Signature::~LibGit_Signature() noexcept
{
	if (_handle)
		//	git_signature_free(const_cast<git_signature*>(_handle));

		_handle = nullptr;
}

namespace
{
std::wstring getWStringFromMBPtr(const char* ptr)
{
	const size_t target_size = std::strlen(ptr) + 1;
	auto name_wstr = std::unique_ptr<wchar_t[]>(new wchar_t[target_size]);

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
	if (!handle)
		throw std::invalid_argument("Handle cannot be nullptr.");

	const auto id_ptr = git_commit_id(handle);
	_id = *id_ptr;
	// delete id_ptr; // TODO probably bug
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
		//	git_commit_free(_handle);

		_handle = nullptr;
}

::git_oid LibGit_Commit::id() const
{
	return _id;
}

std::string LibGit_Commit::getShortMessage() const
{
	const auto src_msg = git_commit_summary(_handle);
	if (!src_msg)
		return {};

	std::string s(src_msg);

	// const auto wstr = getWStringFromMBPtr(src_msg);
	//	delete[] src_msg; // TODO probably bug
	return s;
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

DateTime LibGit_Commit::getTime() const
{
	const auto git_time = git_commit_time(_handle);
	auto point = std::chrono::system_clock::from_time_t(git_time);

	return std::chrono::time_point_cast<std::chrono::milliseconds>(point);
}

LibGit_Ref::LibGit_Ref(git_reference* handle) : _handle(handle)
{
	if (!handle)
		throw std::invalid_argument("Handle cannot be nullptr.");
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
	if (git_revwalk_new(&_handle, repository._handle) != 0)
	{
		auto err = git_error_last();
		if (!err)
			throw LibGit_Exception("", 0);

		const std::string msg(err->message);

		throw LibGit_Exception(msg, err->klass);
	}
}

LibGit_RevisionWalker::~LibGit_RevisionWalker() noexcept
{
	if (_handle)
		git_revwalk_free(_handle);

	_handle = nullptr;
}

void LibGit_RevisionWalker::reset()
{
	if (git_revwalk_reset(_handle) != 0)
	{
		auto err = git_error_last();
		if (!err)
			throw LibGit_Exception("", 0);

		const std::string msg(err->message);

		throw LibGit_Exception(msg, err->klass);
	}
}

void LibGit_RevisionWalker::setReference(const LibGit_Ref& reference)
{
	reset();

	if (git_revwalk_push_ref(_handle, reference.name().c_str()) != 0)
	{
		auto err = git_error_last();
		if (!err)
			throw LibGit_Exception("", 0);

		const std::string msg(err->message);

		throw LibGit_Exception(msg, err->klass);
	}

	if (git_revwalk_sorting(_handle, git_sort_t::GIT_SORT_TIME | git_sort_t::GIT_SORT_REVERSE) != 0)
	{
		auto err = git_error_last();
		if (!err)
			throw LibGit_Exception("", 0);

		const std::string msg(err->message);

		throw LibGit_Exception(msg, err->klass);
	}
}

std::optional<LibGit_Commit::Ptr> LibGit_RevisionWalker::next()
{
	git_oid id;
	const auto result = git_revwalk_next(&id, _handle);

	if (result == GIT_ITEROVER)
		return {};

	if (result != 0)
	{
		auto err = git_error_last();
		if (!err)
			throw LibGit_Exception("", 0);

		const std::string msg(err->message);

		throw LibGit_Exception(msg, err->klass);
	}

	return LibGit_Commit::Ptr(new LibGit_Commit(_repository, id));
}

LibGit_BranchIterator::LibGit_BranchIterator(const LibGit_Repository& repository, git_branch_t filter_by_type)
{
	if (git_branch_iterator_new(&_handle, repository._handle, filter_by_type) != 0)
	{
		auto err = git_error_last();
		if (!err)
			throw LibGit_Exception("", 0);

		const std::string msg(err->message);

		throw LibGit_Exception(msg, err->klass);
	}
}

LibGit_BranchIterator::~LibGit_BranchIterator() noexcept
{
	if (_handle)
		git_branch_iterator_free(_handle);

	_handle = nullptr;
}

std::optional<LibGit_Ref::Ptr> LibGit_BranchIterator::next()
{
	git_reference* ref;
	git_branch_t type;

	const auto result = git_branch_next(&ref, &type, _handle);

	if (result == GIT_ITEROVER)
		return {};

	if (result != 0)
	{
		auto err = git_error_last();
		if (!err)
			throw LibGit_Exception("", 0);

		const std::string msg(err->message);

		throw LibGit_Exception(msg, err->klass);
	}

	return LibGit_Ref::Ptr(new LibGit_Ref(ref));
}

LibGit_Remote::LibGit_Remote(git_remote* handle) : _handle(handle)
{
	if (!handle)
		throw std::invalid_argument("Handle cannot be nullptr.");
}

LibGit_Remote::~LibGit_Remote() noexcept
{
	if (_handle)
		git_remote_free(_handle);

	_handle = nullptr;
}

std::string LibGit_Remote::remoteNameOnServer() const
{
	const char* url = git_remote_url(_handle);
	if (!url)
		return {};

	std::string full_url(url);

	const auto index = full_url.find_last_of('/');
	if (index < 0 || index == full_url.length() - 1)
		return full_url;

	return full_url.substr(index + 1);
}

LibGit_Repository::LibGit_Repository(const LibGit& /* unused */)
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
	if (_handle)
		return false;

	int err = git_repository_open(&_handle, dir.generic_string().c_str());

	auto e = git_error_last();
	std::string s(e->message);

	return err == 0 && _handle;
}

bool LibGit_Repository::fetch(const LibGit_Remote& remote)
{
	return git_remote_fetch(remote._handle, nullptr, nullptr, "Fetch in Raport PKUP - before generating raport.") == 0;
}

bool LibGit_Repository::prune(const LibGit_Remote& remote)
{
	return git_remote_prune(remote._handle, nullptr) == 0;
}

namespace
{
std::vector<LibGit_Ref::Ptr> enumerateBranches(const LibGit_Repository& repository, git_branch_t filter_by_type)
{
	std::vector<LibGit_Ref::Ptr> result;
	LibGit_BranchIterator iterator(repository, filter_by_type);

	while (auto ptr = iterator.next())
		if (ptr->operator bool())
			result.push_back(*ptr);

	return result;
}
} // namespace

std::vector<LibGit_Ref::Ptr> LibGit_Repository::enumerateAllLocalBranches() const
{
	return enumerateBranches(*this, GIT_BRANCH_LOCAL);
}

std::vector<LibGit_Ref::Ptr> LibGit_Repository::enumerateAllRemoteBranches() const
{
	return enumerateBranches(*this, GIT_BRANCH_REMOTE);
}

std::vector<LibGit_Remote::Ptr> LibGit_Repository::enumerateRemotes() const
{
	git_strarray arr;
	git_remote_list(&arr, _handle);

	std::vector<LibGit_Remote::Ptr> remotes;

	for (size_t i = 0; i < arr.count; ++i)
	{
		git_remote* remote = nullptr;
		git_remote_lookup(&remote, _handle, arr.strings[i]);

		if (remote)
			remotes.emplace_back(new LibGit_Remote(remote));
	}

	git_strarray_dispose(&arr);
	return remotes;
}

LibGit_RevisionWalker::Ptr LibGit_Repository::createWalker() const
{
	return std::make_shared<LibGit_RevisionWalker>(*this);
}

Author LibGit_Repository::getAuthorFromConfig() const
{
	git_signature* result = nullptr;
	if (git_signature_default(&result, _handle) != 0 || !result)
		return {};

	const LibGit_Signature signature(result);
	Author author;
	author.name = signature.getName();
	author.email = signature.getEmail();
	return author;
}

LibGit::LibGit()
{
	git_libgit2_init();
}

LibGit::~LibGit() noexcept
{
	return; // TODO
			// git_libgit2_shutdown();
}

bool LibGit::checkRepositoryIsValid(const std::filesystem::path& path) const
{
	const auto str = path.generic_string();
	const auto result = git_repository_open_ext(nullptr, str.c_str(), 0, nullptr);

	if (result == 0)
		return true;

	if (result == GIT_ENOTFOUND)
		return false;

	auto err = git_error_last();
	if (!err)
		throw LibGit_Exception("", 0);

	const std::string msg(err->message);

	throw LibGit_Exception(msg, err->klass);
}

LibGit_Repository::Ptr LibGit::openRepository(const std::filesystem::path& path) const
{
	LibGit_Repository::Ptr repo(new LibGit_Repository(*this));

	if (!repo->tryOpen(path))
		return {};

	return repo;
}

std::optional<std::filesystem::path> LibGit::detectRepositoryRootPath(const std::filesystem::path& path) const
{
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
}
} // namespace RaportPKUP
