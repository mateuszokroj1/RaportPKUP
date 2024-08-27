#pragma once

#include <array>
#include <string>

#include "Author.hpp"
#include "base.hpp"

namespace RaportPKUP
{
struct Commit
{
	std::wstring message;
	DateTime datetime;

	// NULL-terminated SHA string
	using Id = std::array<char, 8>;
	Id id;

	std::string branch_name;
	std::string repo_name;
	Author author;
};
} // namespace RaportPKUP
