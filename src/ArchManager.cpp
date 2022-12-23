#include "ArchManager.hpp"

namespace DarkDescent
{
	void ArchManager::onInitialize()
	{

	}

	void ArchManager::onTerminate()
	{

	}

	void ArchManager::registerArch(Bitmask bitmask)
	{
		assert(!arches_.contains(bitmask));

		std::vector<Component*> c;
		std::size_t size = 0;

		for (std::size_t i = 0; i < bitmask; i++)
		{
			std::size_t b = 1ULL << i;
			if ((bitmask & b) == b)
			{
				auto& component = components_.at(i);
				size += component.size;
				c.emplace_back(std::addressof(component));
			}
		}
		arches_.emplace(std::piecewise_construct, std::forward_as_tuple(bitmask), std::forward_as_tuple(*this, bitmask, size, std::move(c)));
	}

	Arch* ArchManager::getArch(Bitmask bitmask)
	{
		if (!arches_.contains(bitmask))
			registerArch(bitmask);
		return std::addressof(arches_.at(bitmask));
	}

	Entity ArchManager::allocEntity(Bitmask bitmask)
	{
		return getArch(bitmask)->alloc();
	}
}