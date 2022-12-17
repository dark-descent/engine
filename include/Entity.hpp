#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	struct Entity
	{
		Entity(std::size_t bufferIndex, std::size_t index):
			bufferIndex(bufferIndex),
			index(index)
		{

		}

		std::size_t bufferIndex;
		std::size_t index;
	};
};