#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <source_location>
#include <filesystem>
#include <regex>

#include <GitRepository.hpp>

using namespace std::filesystem;

class Lib_GitRepository : public testing::Test
{
protected:
	Lib_GitRepository()
	{
		valid_path =
			path(std::source_location::current().file_name()).parent_path();

		invalid_path = valid_path.root_path();
	}

	path valid_path;
	path invalid_path;
	std::regex re;
};

TEST_F(Lib_GitRepository, checkIsValidPath_whenPutCurrentDirectory_shouldReturnTrue)
{
	ASSERT_TRUE(RaportGen::GitRepository::checkIsValidPath(valid_path));
}

TEST_F(Lib_GitRepository, checkIsValidPath_whenPutBadPath_shouldReturnFalse)
{
	ASSERT_FALSE(RaportGen::GitRepository::checkIsValidPath(invalid_path));
}

TEST_F(Lib_GitRepository, getSystemConfigAuthor_shouldReturnValid)
{
	const auto result = RaportGen::GitRepository::getSystemConfigAuthor();
	ASSERT_FALSE(result.name.empty());
	ASSERT_FALSE(result.email.empty());
	
	//EXPECT_THAT(result.email, L"^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");
}

TEST_F(Lib_GitRepository, getCommits_shouldReturnValid)
{
	RaportGen::GitRepository repo(valid_path);
	const std::chrono::year_month_day ymd(std::chrono::year(2023), std::chrono::month(1), std::chrono::day(1));
	const auto vec = repo.getCommitsFromTimeRange(std::chrono::sys_days(ymd), std::chrono::system_clock::now(), {L"Mateusz Okrój",L"mateuszokroj1@gmail.com"});

	ASSERT_TRUE(vec->size() > 0);
}

TEST_F(Lib_GitRepository, test)
{
	ASSERT_NO_THROW(RaportGen::GitRepository::test());
}