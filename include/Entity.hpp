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
		/**
		 * @brief The index to get the correct buffer
		 */
		std::size_t bufferIndex;
		/**
		 * @brief The index into the buffer.
		 */
		std::size_t index;
	};
};