#pragma once

#include "Entity.hpp"
#include "PersistentAllocator.hpp"
#include "Component.hpp"

namespace DarkDescent
{
	class Arch;
	struct ComponentInfo;

	class GameObject
	{
	public:
		GameObject(Arch* arch, Entity&& entity);
		~GameObject();

		void addComponent(const ComponentInfo& component);
		void addComponent(const std::size_t componentIndex);

		template<ExtendsComponent T, typename... Args>
		T* addComponent(Args&&... args)
		{
			addComponent(T::index);
			T* ptr = getComponent<T>();
			return new (ptr) T(std::forward<Args>(args)...);
		}

		void* getComponentRaw(const ComponentInfo& component) const;
		void* getComponentRaw(const std::size_t bitmask) const;

		template<ExtendsComponent T>
		T* getComponent()
		{
			return static_cast<T*>(getComponentRaw(T::bitmask));
		}
	
		Arch* arch;
		Entity entity;
	};

	using GameObjectHandle = PersistentAllocator<GameObject>::Handle;
}