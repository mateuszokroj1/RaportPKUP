#pragma once

#include <string>

namespace RaportPKUP
{
struct Author
{
	Author() = default;
	Author(const std::wstring& name, const std::wstring& email) : name(name), email(email)
	{
	}

	std::wstring name;
	std::wstring email;
};
} // namespace RaportPKUP
