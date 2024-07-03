#pragma once

#include <string>

#include "Author.hpp"
#include "base.hpp"

namespace RaportPKUP
{
struct Commit
{
	std::string message;
	DateTime datetime;

	// NULL-terminated SHA string
	char id[8];

	std::string branch_name;
	std::string repo_name;
	Author author;
};
} // namespace RaportPKUP
