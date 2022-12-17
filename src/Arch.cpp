#include "Arch.hpp"
#include "ComponentInfo.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	Arch::Arch(ArchManager& manager, std::size_t bitmask, std::size_t size, std::size_t level, std::initializer_list<ComponentInfo*> components):
		manager_(manager),
		bitmask(bitmask),
		size(size),
		level(level),
		bufferPool_(size, 1024),
		components_(components),
		componentOffsets_(),
		prevArches_(),
		nextArches_()
	{
		std::sort(components_.begin(), components_.end(), [](ComponentInfo* a, ComponentInfo* b) { return a->index - b->index; });
		std::size_t offset = 0;
		for (auto& component : components_)
		{
			componentOffsets_.emplace_back(offset);
			offset += component->size;
		}
	}

	std::size_t Arch::getComponentOffset(const ComponentInfo& component)
	{
		std::size_t i = 0;
		for (auto& c : components_)
		{
			if(c->bitmask == component.bitmask)
				return componentOffsets_[i];
			i++;
		}
		throw TraceException("Cannot get component!");
	}

	void* Arch::getComponent(const Entity& entity, const ComponentInfo& component)
	{
		return bufferPool_.getRaw(entity) + getComponentOffset(component);
	}

	Arch& Arch::addComponent(Entity& entity, const ComponentInfo& component)
	{
		// TODO
		return *this;
	}

	Entity Arch::allocEntity()
	{
		return Entity(0, 0);
	}

	void Arch::freeEntity(const Entity&)
	{

	}
}