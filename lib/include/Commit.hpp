#pragma once

#include <string>

#include "Author.hpp"
#include "base.hpp"

namespace RaportGen
{
struct Commit
{
	std::string message;
	DateTime datetime;
	std::string id;
	Author author;
};
} // namespace RaportGen
