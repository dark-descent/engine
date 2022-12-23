#pragma once

#include "pch.hpp"
#include "ArchBufferPool.hpp"
#include "PersistentAllocator.hpp"

namespace DarkDescent
{
	struct Entity;
	class ComponentInfo;
	class ArchManager;
	class GameObject;

	using GameObjectHandle = PersistentAllocator<GameObject>::Handle;

	class Arch
	{
		private:
			static std::size_t calculateArchSize(const std::vector<const ComponentInfo*>& components);

	public:
		Arch(ArchManager& manager, std::vector<const ComponentInfo*> components);

		GameObjectHandle* getGameObject(const Entity& entity);

		/**
		 * @brief Gets a component that belongs to an entity.
		 *
		 * @param entity The entity from which to fetch the component.
		 * @param component The component to fetch.
		 * @return void* a pointer to the components data.
		 */
		char* getComponent(const Entity& entity, const ComponentInfo& component);

		/**
		 * @brief Adds a component to an entity.
		 *
		 * @param entity The entity.
		 * @param component The component which should be added to the component.
		 * @return Arch& The new arch to which the entity belongs
		 */
		Arch& addComponent(Entity& entity, const ComponentInfo& component);

		Entity allocEntity();
		void freeEntity(const Entity&);

	private:
		std::size_t getComponentOffset(const ComponentInfo& component);
		Arch& getNextArch(const ComponentInfo& component);

		Entity copyEntityFrom(Arch& arch, const Entity& entity);

	private:
		ArchManager& manager_;

	public:
		const std::size_t bitmask;
		const std::size_t size;
		const std::size_t level;

	private:
		ArchBufferPool bufferPool_;
		std::vector<const ComponentInfo*> components_;
		std::vector<std::size_t> componentOffsets_;
		std::vector<Arch*> prevArches_;
		std::vector<Arch*> nextArches_;
	};
}