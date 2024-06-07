#pragma once

#include "../include/Author.hpp"
#include "base.hpp"
#include <list>

namespace RaportPKUP
{
class LibGit_Signature
{
  public:
	using Ptr = std::shared_ptr<LibGit_Signature>;

	~LibGit_Signature();
	LibGit_Signature() = delete;
	LibGit_Signature(const LibGit_Repository &, const std::string &, bool &);

	std::wstring getName() const;
	std::wstring getEmail() const;

  private:
	LibGit_Signature(const LibGit_Repository &, git_reference *);
	COPY_CONSTRUCTOR(LibGit_Signature) = delete;

	git_signature *_handle = nullptr;
};

class LibGit_Commit
{
  public:
	class ID
	{
		ID() = delete;

	  private:
		ID(const LibGit_Repository &, git_oid *);

		const git_oid _raw;
	};

	friend class LibGit_Repository;

	using Ptr = std::shared_ptr<LibGit_Commit>;

	~LibGit_Commit();
	LibGit_Commit() = delete;

	ID id() const;
	std::string getShortMessage() const;
	Author getAuthor();
	std::chrono::system_clock::time_point getTime() const;

  private:
	LibGit_Commit(const LibGit_Repository &, git_commit *);
	LibGit_Commit(const LibGit_Repository &, const ID &id);
	COPY_CONSTRUCTOR(LibGit_Commit) = delete;

	git_oid *oid();

	const LibGit_Repository &_repo;
	git_commit *_handle = nullptr;
	const ID _id;
};

class LibGit_Ref
{
	friend class LibGit_Ref;

  public:
	using Ptr = std::shared_ptr<LibGit_Ref>;

	~LibGit_Ref();
	LibGit_Ref() = delete;
	LibGit_Ref(const LibGit_Repository &, const std::string &, bool &);

	LibGit_RevisionWalker::Ptr createWalker() const;

  private:
	LibGit_Ref(const LibGit_Repository &, git_reference *);
	COPY_CONSTRUCTOR(LibGit_Ref) = delete;

	git_reference *_handle = nullptr;
};

class LibGit_RevisionWalker : public IEnumerator<LibGit_Commit>
{
  public:
	friend class LibGit_Repository;
	friend class LibGit_Ref;

	using Ptr = std::shared_ptr<LibGit_RevisionWalker>;

	~LibGit_RevisionWalker() override;
	LibGit_RevisionWalker() = delete;
	LibGit_RevisionWalker(const LibGit_Repository &);

	void reset();
	void setReference(const LibGit_Ref &);

	std::optional<LibGit_Commit> next() override;

  private:
	COPY_CONSTRUCTOR(LibGit_RevisionWalker) = delete;

	git_revwalk *_handle = nullptr;
};

class LibGit_Repository
{
	friend class LibGit;

  public:
	using Ptr = std::shared_ptr<LibGit_Repository>;

	~LibGit_Repository() noexcept;

	bool tryOpen(const std::filesystem::path &dir) noexcept;

	void fetch();

	std::list<LibGit_Ref> enumerateAllLocalBranches() const;
	std::list<LibGit_Ref> enumerateAllRemoteBranches() const;

  private:
	LibGit_Repository(std::weak_ptr<LibGit>);
	COPY_CONSTRUCTOR(LibGit_Repository) = delete;

	git_repository *_handle = nullptr;
	const std::weak_ptr<LibGit> library;
};

class LibGit
{
  private:
	LibGit();

  public:
	~LibGit();

	bool checkRepositoryIsValid(const std::filesystem::path &) const;

	LibGit_Repository::Ptr openRepository(const std::filesystem::path &) const;
};
} // namespace RaportPKUP
