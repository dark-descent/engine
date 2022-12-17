#pragma once

#include "Entity.hpp"
#include "PersistentAllocator.hpp"

namespace DarkDescent
{
	class Arch;

	class GameObject
	{
	public:
		GameObject(Arch& arch, Entity&& entity);
		~GameObject();

		Arch& arch;
		Entity entity;
	};
	
	using GameObjectHandle = PersistentAllocator<GameObject>::Handle;
}