#pragma once

#include "pch.hpp"
#include "ArchBufferPool.hpp"

namespace DarkDescent
{
	class Entity;
	class ComponentInfo;

	class Arch
	{
	public:
		Arch(std::size_t bitmask, std::size_t size, std::size_t level, std::initializer_list<ComponentInfo*> components);

		/**
		 * @brief Gets a component that belongs to an entity.
		 *
		 * @param entity The entity from which to fetch the component.
		 * @param component The component to fetch.
		 * @return void* a pointer to the components data.
		 */
		void* getComponent(const Entity& entity, const ComponentInfo& component);

		/**
		 * @brief Adds a component to an entity.
		 *
		 * @param entity The entity.
		 * @param component The component which should be added to the component.
		 * @return Arch* The new arch to which the entity belongs
		 */
		Arch* addComponent(const Entity& entity, const ComponentInfo& component);

		Entity allocEntity();
		void freeEntity(const Entity&);


	public:
		const std::size_t bitmask;
		const std::size_t size;
		const std::size_t level;

	private:
		ArchBufferPool bufferPool_;
		std::vector<std::size_t> componentOffsets_;
		std::vector<Arch*> prevArches_;
		std::vector<Arch*> nextArches_;
	};
}