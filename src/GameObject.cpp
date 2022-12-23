#include "GameObject.hpp"
#include "Arch.hpp"

namespace DarkDescent
{
	GameObject::GameObject(Arch& arch, Entity&& entity):
		arch(arch),
		entity(entity)
	{

	}

	GameObject::~GameObject()
	{

	}

	void* GameObject::addComponent(const ComponentInfo& component)
	{
		auto a = arch.addComponent(entity, component);
		return a.getComponent(entity, component);
	}

	void* GameObject::getComponent(const ComponentInfo& component)
	{
		return arch.getComponent(entity, component);
	}
}