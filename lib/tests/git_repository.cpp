#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <regex>
#include <source_location>

#include <Author.hpp>
#include <Commit.hpp>
#include <GitRepository.hpp>
#include <LibGit.hpp>

using namespace std::filesystem;

class Lib_GitRepository : public testing::Test
{
  protected:
	Lib_GitRepository()
	{
		valid_path = path(std::source_location::current().file_name()).parent_path();

		invalid_path = valid_path.root_path();
	}

	path valid_path;
	path invalid_path;
	std::regex re;
};

/* TEST_F(Lib_GitRepository, checkIsValidPath_whenPutCurrentDirectory_shouldReturnTrue)
{
	//ASSERT_TRUE(RaportPKUP::GitRepository::checkIsValidPath(valid_path));
}

TEST_F(Lib_GitRepository, checkIsValidPath_whenPutBadPath_shouldReturnFalse)
{
	ASSERT_FALSE(RaportPKUP::GitRepository::checkIsValidPath(invalid_path));
}

TEST_F(Lib_GitRepository, getSystemConfigAuthor_shouldReturnValid)
{
	const auto result = RaportPKUP::GitRepository::getSystemConfigAuthor();
	ASSERT_FALSE(result.name.empty());
	ASSERT_FALSE(result.email.empty());

	// EXPECT_THAT(result.email, L"^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");
}

TEST_F(Lib_GitRepository, getCommits_shouldReturnValid)
{
	RaportPKUP::GitRepository repo(valid_path);
	const std::chrono::year_month_day ymd(std::chrono::year(2023), std::chrono::month(1), std::chrono::day(1));
	const auto vec = repo.getCommitsFromTimeRange(std::chrono::sys_days(ymd), std::chrono::system_clock::now(),
												  {L"Mateusz Okrój", L"mateuszokroj1@gmail.com"});

	ASSERT_TRUE(vec->size() > 0);
}*/
