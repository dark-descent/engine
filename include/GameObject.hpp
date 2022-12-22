#pragma once

#include "Entity.hpp"
#include "PersistentAllocator.hpp"
#include "ComponentInfo.hpp"
#include "Arch.hpp"

namespace DarkDescent
{
	class Arch;

	class GameObject
	{
	public:
		GameObject(Arch& arch, Entity&& entity);
		~GameObject();

		template<typename T>
		T& addComponent(ComponentInfo& component)
		{
			assert(component.size == sizeof(T));
			arch = arch.addComponent(entity, component);
			return *static_cast<T*>(arch.getComponent(entity, component));
		}

		template<typename T>
		T& getComponent(ComponentInfo& component)
		{
			assert(component.size == sizeof(T));
			return *static_cast<T*>(arch.getComponent(entity, component));
		}

		Arch& arch;
		Entity entity;
	};
	
	using GameObjectHandle = PersistentAllocator<GameObject>::Handle;
}