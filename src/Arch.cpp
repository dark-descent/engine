#include "Arch.hpp"

namespace DarkDescent
{
	Arch::Arch(std::size_t bitmask, std::size_t size, std::size_t level, std::initializer_list<ComponentInfo*> components):
		bitmask(bitmask),
		size(size),
		level(level),
		bufferPool_(size, 1024),
		componentOffsets_(),
		prevArches_(),
		nextArches_()
	{

	}

	void* Arch::getComponent(const Entity& entity, const ComponentInfo& component)
	{
		return nullptr;
	}

	Arch* Arch::addComponent(const Entity& entity, const ComponentInfo& component)
	{
		return nullptr;
	}

	Entity Arch::allocEntity()
	{
		return Entity(0, 0);
	}

	void Arch::freeEntity(const Entity&)
	{

	}
}