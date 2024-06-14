#pragma once

#include <string>

#include "Author.hpp"
#include "base.hpp"

namespace RaportPKUP
{
struct Commit
{
	std::wstring message;
	DateTime datetime;
	std::string id;
	std::string branch_name;
	Author author;
};
} // namespace RaportPKUP
