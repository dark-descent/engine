#pragma once

#include "Entity.hpp"
#include "PersistentAllocator.hpp"
#include "ComponentInfo.hpp"

namespace DarkDescent
{
	class Arch;
	class GameObject
	{
	public:
		GameObject(Arch& arch, Entity&& entity);
		~GameObject();

	private:
		void* addComponent(const ComponentInfo& component);
		void* getComponent(const ComponentInfo& component);

	public:
		template<typename T>
		T& addComponent(const ComponentInfo& component)
		{
			assert(component.size == sizeof(T));
			return *static_cast<T*>(addComponent(component));
		}

		template<typename T>
		T& getComponent(const ComponentInfo& component)
		{
			assert(component.size == sizeof(T));
			return *static_cast<T*>(getComponent(component));
		}

		Arch& arch;
		Entity entity;
	};

	using GameObjectHandle = PersistentAllocator<GameObject>::Handle;
}