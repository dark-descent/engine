#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class ComponentInfo
	{
	public:
		static inline std::size_t bitmaskFromComponents(const std::vector<const ComponentInfo*>& components)
		{
			std::size_t bitmask = 0;
			for (auto c : components)
				bitmask |= c->bitmask;
			return bitmask;
		}

		std::size_t size;
		std::size_t index;
		std::size_t bitmask;

		ComponentInfo(std::size_t size, std::size_t index):
			size(size),
			index(index),
			bitmask(1ULL << index)
		{ }
	};
}