#pragma once

namespace RaportPKUP
{

class LibGit_Ref
{
	friend class LibGit_Repository;

  public:
	~LibGit_Ref();
	LibGit_Ref(const LibGit_Repository &) = delete;

  private:
	LibGit_Ref();

	git_reference *_handle = nullptr;
};

class LibGit_Repository
{
	friend class LibGit;

  public:
	~LibGit_Repository();
	LibGit_Repository(const LibGit_Repository &) = delete;

  private:
	LibGit_Repository();

	git_repository *_handle = nullptr;
};

class LibGit
{
  public:
	~LibGit();

	static void init();

	static inline std::shared_ptr<LibGit> getInstance()
	{
		return _instance;
	}

	bool checkRepositoryIsValid(const std::filesystem::path &) const;

	LibGit_Repository openRepository(const std::filesystem::path &) const;

  private:
	LibGit();

	static std::shared_ptr<LibGit> _instance;
};
} // namespace RaportPKUP
