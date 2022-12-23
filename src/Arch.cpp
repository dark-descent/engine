#include "Arch.hpp"
#include "ComponentInfo.hpp"
#include "TraceException.hpp"
#include "ArchManager.hpp"
#include "GameObject.hpp"

namespace DarkDescent
{
	std::size_t Arch::calculateArchSize(const std::vector<const ComponentInfo*>& components)
	{
		std::size_t size = 0;
		for (auto& c : components)
			size += c->size;
		return size;
	}

	Arch::Arch(ArchManager& manager, std::vector<const ComponentInfo*> components):
		manager_(manager),
		bitmask(ComponentInfo::bitmaskFromComponents(components)),
		size(calculateArchSize(components)),
		level(components.size()),
		bufferPool_(size, 1024),
		components_(components),
		componentOffsets_(),
		prevArches_(),
		nextArches_()
	{
		std::vector<const ComponentInfo*> sorted(components.size());
		std::partial_sort_copy(std::begin(components), std::end(components), begin(sorted), end(sorted), [](const ComponentInfo* a, const ComponentInfo* b) { return a->index - b->index; });

		std::size_t offset = 0;
		for (auto& component : components_)
		{
			componentOffsets_.emplace_back(offset);
			offset += component->size;
		}
	}

	GameObjectHandle* Arch::getGameObject(const Entity& entity)
	{
		return bufferPool_.getGameObject(entity);
	}

	std::size_t Arch::getComponentOffset(const ComponentInfo& component)
	{
		std::size_t i = 0;
		for (auto& c : components_)
		{
			if (c->bitmask == component.bitmask)
				return componentOffsets_[i];
			i++;
		}
		throw TraceException("Cannot get component!");
	}

	Arch& Arch::getNextArch(const ComponentInfo& component)
	{
		auto bm = bitmask | component.bitmask;;
		for (auto& a : nextArches_)
		{
			if (a->bitmask == bm)
				return *a;
		}

		std::vector<const ComponentInfo*> componentInfos = components_;
		componentInfos.emplace_back(std::addressof(component));
		Arch& arch = manager_.getArch(componentInfos);
		nextArches_.emplace_back(std::addressof(arch));
		return arch;
	}

	char* Arch::getComponent(const Entity& entity, const ComponentInfo& component)
	{
		return bufferPool_.getRaw(entity) + getComponentOffset(component);
	}

	Entity Arch::copyEntityFrom(Arch& arch, const Entity& entity)
	{
		const Entity e = allocEntity();
		char* baseFrom = arch.bufferPool_.getRaw(entity);
		char* baseTo = bufferPool_.getRaw(e);
		for(const auto& component : arch.components_)
		{
			const auto offsetFrom = arch.getComponentOffset(*component);
			const auto offsetTo = getComponentOffset(*component);
			memcpy(baseTo + offsetTo, baseFrom + offsetFrom, component->size);
		}
		return e;
	}

	Arch& Arch::addComponent(Entity& entity, const ComponentInfo& component)
	{
		auto& newArch = getNextArch(component);
		auto newEntity = newArch.copyEntityFrom(*this, entity);
		freeEntity(entity);
		entity.bufferIndex = newEntity.bufferIndex;
		entity.index = newEntity.index;
		return newArch;
	}

	Entity Arch::allocEntity()
	{
		return bufferPool_.alloc();
	}

	void Arch::freeEntity(const Entity&)
	{
		// TODO
	}
}