#pragma once

#include "ArchBufferPool.hpp"

namespace DarkDescent
{
	class ArchManager;

	struct Component;
	class Arch;

	struct ComponentOffset
	{
		std::size_t size;
		std::size_t offset;
	};

	struct ArchArm
	{
		Arch* arch;
		std::size_t bitmask;
		std::size_t splitOffset;
		std::size_t size;
		
		ArchArm(Arch* arch, std::size_t bitmask, std::size_t splitOffset, std::size_t size):
			arch(arch),
			bitmask(bitmask),
			splitOffset(splitOffset),
			size(size)
		{ }
	};

	class Arch
	{
	public:
		Arch(ArchManager& archManager, std::size_t bitmask, std::size_t archSize, std::vector<Component*>&& components);
		Arch(const Arch&) = delete;
		Arch(Arch&&) = delete;
		~Arch();

	public:
		const std::size_t bitmask;
		std::size_t archSize;

		Entity alloc();
		void free(const Entity& entity);

		Arch* addComponentToEntity(Entity& entity, const Component& component);
		void* getComponentRaw(const Entity& entity, std::size_t bitmask) const;

		template<typename T>
		T* getComponent(const Entity& entity, std::size_t bitmask) const
		{
			return static_cast<T*>(getComponentRaw(entity, bitmask));
		}

		GameObjectHandle* getGameObjectHandle(const Entity& entity);

		const ArchArm& getNext(const Component& component);
		const ArchArm& getPrev(const Component& component);

		const ComponentOffset& getComponentOffset(const Component& component) const;
		const ComponentOffset& getComponentOffset(std::size_t bitmask) const;

	private:
		ArchManager& archManager_;
		ArchBufferPool bufferPool_;
		std::vector<Component*> components_;
		std::vector<ComponentOffset> offsets_;
		std::vector<ArchArm*> next_;
		std::vector<ArchArm*> prev_;
	};
}