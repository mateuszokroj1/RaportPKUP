#pragma once

#include <list>

#include <git2.h>

#include "../include/Author.hpp"
#include "base.hpp"

namespace RaportPKUP
{
class LibGit_Signature
{
  public:
	using Ptr = std::shared_ptr<LibGit_Signature>;

	~LibGit_Signature() noexcept;
	LibGit_Signature() = delete;
	LibGit_Signature(const git_signature*);

	std::wstring getName() const;
	std::wstring getEmail() const;

  private:
	COPY_CONSTRUCTOR(LibGit_Signature) = delete;

	const git_signature* _handle = nullptr;
};

class LibGit_Repository;

class LibGit_Commit
{
  public:
	friend class LibGit_Repository;
	friend class LibGit_RevisionWalker;

	using Ptr = std::shared_ptr<LibGit_Commit>;

	~LibGit_Commit() noexcept;
	LibGit_Commit() = delete;

	git_oid id() const;
	std::wstring getShortMessage() const;
	Author getAuthor();
	std::chrono::system_clock::time_point getTime() const;

  private:
	LibGit_Commit(git_commit*);
	COPY_CONSTRUCTOR(LibGit_Commit) = delete;
	LibGit_Commit(const LibGit_Repository&, const ::git_oid);

	git_commit* _handle = nullptr;
	git_oid _id;
};

class LibGit_Ref
{
	friend class LibGit_BranchIterator;

  public:
	using Ptr = std::shared_ptr<LibGit_Ref>;

	~LibGit_Ref() noexcept;

	bool tryLoad(const LibGit_Repository&, const std::string&);

	std::string name() const;

  private:
	LibGit_Ref(git_reference*);
	COPY_CONSTRUCTOR(LibGit_Ref) = delete;

	git_reference* _handle = nullptr;
};

class LibGit_RevisionWalker : public IEnumerator<LibGit_Commit::Ptr>
{
  public:
	friend class LibGit_Repository;

	using Ptr = std::shared_ptr<LibGit_RevisionWalker>;

	~LibGit_RevisionWalker() noexcept override;
	LibGit_RevisionWalker() = delete;
	LibGit_RevisionWalker(const LibGit_Repository&);

	void reset();
	void setReference(const LibGit_Ref&);

	std::optional<LibGit_Commit::Ptr> next() override;

  private:
	COPY_CONSTRUCTOR(LibGit_RevisionWalker) = delete;

	const LibGit_Repository& _repository;
	git_revwalk* _handle = nullptr;
};

class LibGit_BranchIterator : public IEnumerator<LibGit_Ref::Ptr>
{
  public:
	using Ptr = std::shared_ptr<LibGit_BranchIterator>;

	~LibGit_BranchIterator() noexcept override;
	LibGit_BranchIterator() = delete;
	LibGit_BranchIterator(const LibGit_Repository&, git_branch_t filter_by_type = git_branch_t::GIT_BRANCH_ALL);

	std::optional<LibGit_Ref::Ptr> next() override;

  private:
	COPY_CONSTRUCTOR(LibGit_BranchIterator) = delete;

	git_branch_iterator* _handle = nullptr;
};

class LibGit_Remote
{
	friend class LibGit_Repository;

  public:
	using Ptr = std::shared_ptr<LibGit_Remote>;

	~LibGit_Remote() noexcept;
	LibGit_Remote() = delete;

  private:
	LibGit_Remote(git_remote*);
	COPY_CONSTRUCTOR(LibGit_Remote) = delete;

	git_remote* _handle;
};

class LibGit_Repository
{
	friend class LibGit;
	friend class LibGit_Commit;
	friend class LibGit_Ref;
	friend class LibGit_RevisionWalker;
	friend class LibGit_BranchIterator;

  public:
	using Ptr = std::shared_ptr<LibGit_Repository>;

	~LibGit_Repository() noexcept;

	bool tryOpen(const std::filesystem::path& dir) noexcept;

	bool fetch(const LibGit_Remote&);
	bool prune(const LibGit_Remote&);

	std::vector<LibGit_Ref::Ptr> enumerateAllLocalBranches() const;
	std::vector<LibGit_Ref::Ptr> enumerateAllRemoteBranches() const;
	std::list<LibGit_Remote> enumerateRemotes() const;

	LibGit_RevisionWalker::Ptr createWalker() const;

	Author getAuthorFromConfig() const;

  private:
	LibGit_Repository(const LibGit&);
	COPY_CONSTRUCTOR(LibGit_Repository) = delete;

	git_repository* _handle = nullptr;
};

class LibGit
{
  public:
	LibGit();

	~LibGit() noexcept;

	bool checkRepositoryIsValid(const std::filesystem::path&) const;

	LibGit_Repository::Ptr openRepository(const std::filesystem::path&) const;
	std::optional<std::filesystem::path> detectRepositoryRootPath(const std::filesystem::path&) const;
};
} // namespace RaportPKUP
