#pragma once

#include "Entity.hpp"
#include "PersistentAllocator.hpp"

namespace DarkDescent
{
	class Arch;
	struct Component;

	class GameObject
	{
	public:
		GameObject(Arch* arch, Entity&& entity);
		~GameObject();

		void addComponent(const Component& component);
		void* getComponentRaw(const Component& component);
	
		template<typename T>
		T* getComponent(const Component& component)
		{
			return static_cast<T*>(getComponentRaw(component));
		}

		Arch* arch;
		Entity entity;
	};

	using GameObjectHandle = PersistentAllocator<GameObject>::Handle;
}