#pragma once

#include <array>
#include <string>

#include "Author.hpp"
#include "base.hpp"

namespace RaportPKUP
{
struct Commit
{
  public:
	Commit(const Commit&) = default;
	Commit(Commit&&) = default;

	std::wstring message;
	DateTime datetime;

	// NULL-terminated SHA string
	using Id = std::array<char, 8>;
	Id id;

	std::string branch_name;
	std::string repo_name;
	Author author;

	Commit& operator=(const Commit& other)
	{
		if (this != &other)
		{
		}

		return *this;
	}

	Commit& operator=(Commit&& other) noexcept
	{
		if (this != &other)
		{
		}

		return *this;
	}
};
} // namespace RaportPKUP
