#include <Author.hpp>
#include <Commit.hpp>
#include <GitRepository.hpp>
#include <GitRepositoryAccessor.hpp>
#include <GitRepositoryDetector.hpp>
#include <LibGit.hpp>
#include <Version.h>

#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <regex>
#include <source_location>
#include <sys/stat.h>

using namespace std::filesystem;
using namespace RaportPKUP;

constexpr wchar_t EMAIL_REGEX[] = L"^[\\w\\d]+([\\.-][\\w\\d]+)*@[\\w\\d]+([\\.-][\\w\\d]+)*$";
constexpr wchar_t MINIMAL_NAME_REGEX[] = L"^[\\w\\d\\.-]+$";
constexpr wchar_t COMMENT_REGEX[] = L"[A-z0-9]";
constexpr char BRANCH_NAME_REGEX[] = "^[A-z0-9_-]+(\\/[A-z0-9_-]+)*$";

class DetectorMock : public GitRepositoryDetector
{
  public:
	DetectorMock(GitRepositoryAccessor& accessor) : _accessor(accessor)
	{
	}

  private:
	GitRepositoryAccessor& _accessor;

	const GitRepositoryAccessor& getAccessorImpl() const override
	{
		return _accessor;
	}
};

class GitRepositoryTest : public testing::Test
{
  protected:
	GitRepositoryTest() : accessor(std::make_shared<GitRepositoryAccessor>()), detector_mock(*accessor)
	{
		valid_path = path(GIT_REPOSITORY_ROOT);

		invalid_path = valid_path.root_path();
	}

	path valid_path;
	path invalid_path;
	Ptr<GitRepositoryAccessor> accessor;
	DetectorMock detector_mock;
};

TEST_F(GitRepositoryTest, checkIsValidPath_whenValueIsValid_shouldReturnTrue)
{
	const auto result = detector_mock.detect(valid_path).get();

	ASSERT_TRUE(result.has_value());

	struct stat s;
	const auto result2 = stat(result->generic_string().c_str(), &s) == 0;

	ASSERT_TRUE(result2);
	ASSERT_TRUE(s.st_mode & S_IFDIR);
}

TEST_F(GitRepositoryTest, checkIsValidPath_whenValueIsInvalid_shouldReturnFalse)
{
	const auto result = detector_mock.detect(invalid_path).get();
	ASSERT_FALSE(result.has_value());
}

TEST_F(GitRepositoryTest, getSystemConfigAuthor_shouldReturnValid)
{
	auto repo = accessor->openRepository(valid_path).get();
	ASSERT_TRUE(repo);

	const auto author = repo->getDefaultAuthor();
	if (author)
	{
		ASSERT_FALSE(author->email.empty());
		ASSERT_FALSE(author->name.empty());

		std::wregex test(EMAIL_REGEX);

		ASSERT_TRUE(std::regex_match(author->email, test));
	}
}

TEST_F(GitRepositoryTest, getNameOfRemoteRepo_shouldReturnGitHubName)
{
	auto repo = accessor->openRepository(valid_path).get();
	ASSERT_TRUE(repo);

	ASSERT_STREQ(repo->getNameOfRemoteRepository().c_str(), "RaportPKUP");
}

TEST_F(GitRepositoryTest, getCommits_shouldReturnValid)
{
	auto repo = accessor->openRepository(valid_path).get();
	ASSERT_TRUE(repo);

	const std::chrono::year_month_day from(std::chrono::year(2023), std::chrono::month(1), std::chrono::day(1));
	const std::chrono::year_month_day to(std::chrono::year(2024), std::chrono::month(6), std::chrono::day(1));

	std::map<Commit::Key, Commit> map;
	SynchronizationContainerWrapper<Commit, Commit::Key> container(map);

	repo->getCommitsFromTimeRange(std::chrono::sys_days(from), std::chrono::sys_days(to),
								  {L"Mateusz Okrój", L"mateuszokroj1@gmail.com"}, container);

	ASSERT_EQ(map.size(), 20);

	const auto test_commit = map.cbegin()->second;

	ASSERT_TRUE(std::regex_match(test_commit.branch_name, std::regex(BRANCH_NAME_REGEX)));

	ASSERT_EQ(std::chrono::floor<std::chrono::days>(test_commit.datetime),
			  std::chrono::sys_days(
				  std::chrono::year_month_day(std::chrono::year(2023), std::chrono::month(3), std::chrono::day(30))));
	ASSERT_STREQ(test_commit.repo_name.c_str(), "RaportPKUP");
	// ASSERT_TRUE(std::regex_match(test_commit.message, ));

	const auto& author = test_commit.author;
	// ASSERT_TRUE(std::regex_match(author.email, std::wregex(EMAIL_REGEX)));
}
