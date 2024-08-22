#include <Author.hpp>
#include <Commit.hpp>
#include <GitRepository.hpp>
#include <GitRepositoryAccessor.hpp>
#include <GitRepositoryDetector.hpp>
#include <LibGit.hpp>

#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <regex>
#include <source_location>
#include <sys/stat.h>

using namespace std::filesystem;
using namespace RaportPKUP;

constexpr wchar_t EMAIL_REGEX[] = L"^[\\w\\-\\.]+@([\\w\\-]+\\.)+[\\w\\-]{2,4}$";

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

		repo = std::dynamic_pointer_cast<GitRepository>(accessor->openRepository(valid_path).get());
	}

	path valid_path;
	path invalid_path;
	Ptr<GitRepository> repo;
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
	const auto author = repo->getDefaultAuthor();

	ASSERT_TRUE(author.has_value());
	ASSERT_FALSE(author->email.empty());
	ASSERT_FALSE(author->name.empty());

	std::wregex test(EMAIL_REGEX);

	ASSERT_TRUE(std::regex_match(author->email, test));
}

TEST_F(GitRepositoryTest, getNameOfRemoteRepo_shouldReturnGitHubName)
{
	ASSERT_STREQ(repo->getNameOfRemoteRepository().c_str(), "RaportPKUP");
}

TEST_F(GitRepositoryTest, getCommits_shouldReturnValid)
{
	const std::chrono::year_month_day ymd(std::chrono::year(2023), std::chrono::month(1), std::chrono::day(1));
	const auto vec = repo->getCommitsFromTimeRange(std::chrono::sys_days(ymd), std::chrono::system_clock::now(),
												   {L"Mateusz Okrój", L"mateuszokroj1@gmail.com"})
						 .get();

	ASSERT_GT(vec.size(), 0);
}
