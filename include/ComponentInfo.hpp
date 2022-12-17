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

		template<typename T>
		ComponentInfo(std::size_t index):
			size(sizeof(T)),
			index(index),
			bitmask(1 << index)
		{ }
	};
}