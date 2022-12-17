#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class ComponentInfo
	{
	public:
		const std::size_t size;
		const std::size_t index;
		const std::size_t bitmask;

		ComponentInfo(std::size_t size, std::size_t index):
			size(size),
			index(index),
			bitmask(1ULL << index)
		{ }
	};
}