#pragma once

#include <list>

#include <git2.h>

#include "../include/Author.hpp"
#include "base.hpp"

namespace RaportPKUP
{
class LibGit_Exception : public std::runtime_error
{
  public:
	explicit LibGit_Exception(const std::string& message, int error_code)
		: std::runtime_error(message), error_code(error_code)
	{
	}

  private:
	int error_code;
};

class LibGit_Signature final
{
  public:
	LibGit_Signature(const git_signature*);

	std::wstring getName() const;
	std::wstring getEmail() const;

  private:
	const git_signature* _handle = nullptr;
};

class LibGit_Repository;

class LibGit_Commit final
{
  public:
	friend class LibGit_Repository;
	friend class LibGit_RevisionWalker;

	~LibGit_Commit() noexcept;
	LibGit_Commit() = delete;
	COPY_CONSTRUCTOR(LibGit_Commit) = delete;

	git_oid id() const;
	std::wstring getShortMessage() const;
	Author getAuthor();
	DateTime getTime() const;

  private:
	LibGit_Commit(git_commit*);
	explicit LibGit_Commit(const LibGit_Repository&, const ::git_oid);

	git_commit* _handle = nullptr;
	git_oid _id;
};

class LibGit_Ref final
{
	friend class LibGit_BranchIterator;

  public:
	~LibGit_Ref() noexcept;
	COPY_CONSTRUCTOR(LibGit_Ref) = delete;

	bool tryLoad(const LibGit_Repository&, const std::string&);

	std::string name() const;

  private:
	LibGit_Ref(git_reference*);

	git_reference* _handle = nullptr;
};

class LibGit_RevisionWalker final : public IEnumerator<Ptr<LibGit_Commit>>
{
  public:
	friend class LibGit_Repository;

	~LibGit_RevisionWalker() noexcept override;
	LibGit_RevisionWalker() = delete;
	explicit LibGit_RevisionWalker(const LibGit_Repository&);
	COPY_CONSTRUCTOR(LibGit_RevisionWalker) = delete;

	void reset();
	void setReference(const LibGit_Ref&);

	std::optional<Ptr<LibGit_Commit>> next() override;

  private:
	const LibGit_Repository& _repository;
	git_revwalk* _handle = nullptr;
};

class LibGit_BranchIterator final : public IEnumerator<Ptr<LibGit_Ref>>
{
  public:
	~LibGit_BranchIterator() noexcept override;
	LibGit_BranchIterator() = delete;
	COPY_CONSTRUCTOR(LibGit_BranchIterator) = delete;
	explicit LibGit_BranchIterator(const LibGit_Repository&,
								   git_branch_t filter_by_type = git_branch_t::GIT_BRANCH_ALL);

	std::optional<Ptr<LibGit_Ref>> next() override;

  private:
	git_branch_iterator* _handle = nullptr;
};

class LibGit_Remote final
{
	friend class LibGit_Repository;

  public:
	~LibGit_Remote() noexcept;
	LibGit_Remote() = delete;

	std::string remoteNameOnServer() const;

  private:
	LibGit_Remote(git_remote*);
	COPY_CONSTRUCTOR(LibGit_Remote) = delete;

	git_remote* _handle = nullptr;
};

class LibGit_Repository
{
	friend class LibGit;
	friend class LibGit_Commit;
	friend class LibGit_Ref;
	friend class LibGit_RevisionWalker;
	friend class LibGit_BranchIterator;

  public:
	LibGit_Repository() = default;
	~LibGit_Repository() noexcept;
	COPY_CONSTRUCTOR(LibGit_Repository) = delete;

	bool tryOpen(const std::filesystem::path& dir) noexcept;

	bool fetch(const LibGit_Remote&);
	bool prune(const LibGit_Remote&);

	std::vector<Ptr<LibGit_Ref>> enumerateAllLocalBranches() const;
	std::vector<Ptr<LibGit_Ref>> enumerateAllRemoteBranches() const;
	std::vector<Ptr<LibGit_Remote>> enumerateRemotes() const;

	Ptr<LibGit_RevisionWalker> createWalker() const;

	Author getAuthorFromConfig() const;

  private:
	git_repository* _handle = nullptr;
};
} // namespace RaportPKUP
