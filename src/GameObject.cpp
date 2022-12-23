#include "GameObject.hpp"
#include "Arch.hpp"
#include "Component.hpp"

namespace DarkDescent
{
	GameObject::GameObject(Arch* arch, Entity&& entity):
		arch(arch),
		entity(entity)
	{

	}

	GameObject::~GameObject()
	{

	}

	void GameObject::addComponent(const Component& component)
	{
		arch = arch->addComponentToEntity(entity, component);
	}

	void* GameObject::getComponentRaw(const Component& component)
	{
		return arch->getComponentRaw(entity, component.bitmask);
	}
}