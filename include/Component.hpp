#pragma once

#define COMPONENT_DATA() static std::size_t index; \
static std::size_t bitmask; \
static bool isInitialized;

#define INITIALIZE_COMPONENT_DATA(NAME) std::size_t NAME::index = 0;\
std::size_t NAME::bitmask = 0;\
bool NAME::isInitialized = false; 

namespace DarkDescent
{
	struct Component
	{
		std::size_t size;
		std::size_t index;
		std::size_t bitmask;
	};
}