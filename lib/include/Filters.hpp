#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <vector>

namespace RaportPKUP
{
namespace Filters
{
template <typename T>
concept Equatable = requires(T a, T b) { a == b; };

template <typename DataType, Equatable KeyType>
void unique(std::vector<DataType>& items, const std::function<KeyType(const DataType&)>& key_selector)
{
	for (int position = 0; position < items.size() - 1; ++position)
	{
		for (int offset = 1; position + offset < items.size(); ++offset)
		{
			if (key_selector(items[position]) == key_selector(items[position + offset]))
				items.erase(items.begin() + position + offset);
		}
	}
}

template <typename T> void removeEmptyPointers(std::vector<std::unique_ptr<T>>& items)
{
	auto it = items.begin();

	while (it != items.end())
	{
		if (!*it)
			it = items.erase(it);
		else
			++it;
	}
}

template <typename T>
concept Comparable = requires(T a, T b) {
	a > b;
	a < b;
};

template <std::swappable DataType, Comparable KeyType>
void sortDescending(std::vector<DataType>& items, const std::function<KeyType(const DataType&)>& key_selector)
{
	bool can_break = true;

	do
	{
		can_break = true;

		for (int position = 0; position < items.size() - 1; ++position)
		{
			for (int offset = 1; position + offset < items.size(); ++offset)
			{
				if (key_selector(items[position]) < key_selector(items[position + offset]))
				{
					items[position].swap(items[position + offset]);
					can_break = false;
					break;
				}
			}

			if (!can_break)
				break;
		}
	} while (!can_break);
}
} // namespace Filters
} // namespace RaportPKUP
