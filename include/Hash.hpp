#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	using Hash = std::size_t;

	namespace Hasher
	{
		namespace
		{
			constexpr std::size_t hashString(const char* str)
			{
				std::size_t hash = 5381;
				int c;

				while ((c = *str++))
					hash = ((hash << 5) + hash) * 33 + c;

				return hash;
			}
		}

		constexpr Hash hash(const char* str) { return hashString(str); }
		constexpr bool check(Hash& hashStr, const char* str) { return hash(str) == hashStr; }
	};
};