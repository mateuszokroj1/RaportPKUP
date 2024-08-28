#include <Filters.hpp>

#include <gtest/gtest.h>

using namespace RaportPKUP;

struct TestItem
{
	TestItem(int id, std::string text) : id(id), text(std::move(text))
	{
	}

	void swap(TestItem& other)
	{
		std::swap(id, other.id);
		text.swap(other.text);
	}

	int id;
	std::string text;
};

TEST(Filters, Unique_ShouldRemoveItems)
{
	std::vector<TestItem> items{TestItem{20, "1"}, TestItem{100, "2"}, TestItem{20, "3"}, TestItem{200, "4"},
								TestItem{100, "5"}};

	uint32_t use_counter = 0;
	const size_t expected_use_counter = items.size() * 2;

	const auto key_selector = [&use_counter](const TestItem& item)
	{
		++use_counter;
		return item.id;
	};

	Filters::unique<TestItem, int>(items, key_selector);

	ASSERT_EQ(items.size(), 3);
	ASSERT_EQ(use_counter, expected_use_counter);
}

TEST(Filters, SortDescending_ShouldSwapItems)
{
	std::vector<TestItem> items{TestItem{20, "1"}, TestItem{100, "2"}, TestItem{20, "3"}, TestItem{200, "4"},
								TestItem{100, "5"}};

	uint32_t use_counter = 0;
	const size_t expected_use_counter = items.size();

	const auto key_selector = [&use_counter](const TestItem& item)
	{
		++use_counter;
		return item.id;
	};

	Filters::sortDescending<TestItem, int>(items, key_selector);

	ASSERT_EQ(items.size(), 5);
	ASSERT_EQ(items[0].id, 200);
	ASSERT_EQ(items[4].id, 20);
	ASSERT_GT(use_counter, 5);
}
