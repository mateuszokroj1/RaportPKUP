#pragma once

#include <array>
#include <string>

#include "Author.hpp"
#include "base.hpp"

namespace RaportPKUP
{
struct Commit
{
	struct Id
	{
		Id()
		{
			value.fill(0);
		}

		std::string toString() const
		{
			std::string str;

			for (char i = 0; i < 8; ++i)
				str.append(value.data());

			return str;
		}

		operator std::string() const
		{
			return toString();
		}

		bool operator==(const Id& other) const
		{
			for (char i = 0; i < 8; ++i)
				if (value[i] != other.value[i])
					return false;

			return true;
		}

		bool operator!=(const Id& other) const
		{
			return !operator==(other);
		}

		std::array<char, 8> value;
	};

	struct Key
	{
		Key(const Id& id, const DateTime& time) : id(id), time(time)
		{
		}

		Id id;
		DateTime time;

		bool operator==(const Key& other) const
		{
			return id == other.id;
		}

		bool operator!=(const Key& other) const
		{
			return !operator==(other);
		}

		bool operator<(const Key& other) const
		{
			return time < other.time;
		}

		bool operator>(const Key& other) const
		{
			return time > other.time;
		}
	};

	Commit() = default;
	Commit(const Commit&) = default;
	Commit(Commit&&) = default;

	std::wstring message;
	DateTime datetime;

	// NULL-terminated 7 chars of SHA string
	Id id;

	std::string branch_name;
	std::string repo_name;
	Author author;

	Commit& operator=(const Commit& other)
	{
		if (this != &other)
		{
			message = other.message;
			datetime = other.datetime;
			id = other.id;
			branch_name = other.branch_name;
			repo_name = other.repo_name;
			author = other.author;
		}

		return *this;
	}

	Commit& operator=(Commit&& other) noexcept
	{
		if (this != &other)
		{
			message = std::move(other.message);
			datetime = std::move(other.datetime);
			id = std::move(other.id);
			branch_name = std::move(other.branch_name);
			repo_name = std::move(other.repo_name);
			author = std::move(other.author);
		}

		return *this;
	}
};
} // namespace RaportPKUP
