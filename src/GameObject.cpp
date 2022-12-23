#include "GameObject.hpp"
#include "Arch.hpp"
#include "Component.hpp"
#include "ArchManager.hpp"

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

	void GameObject::addComponent(const ComponentInfo& component)
	{
		arch = arch->addComponentToEntity(entity, component);
	}

	void GameObject::addComponent(const std::size_t componentIndex)
	{
		addComponent(arch->archManager().getComponent(componentIndex));
	}

	void* GameObject::getComponentRaw(const ComponentInfo& component) const
	{
		return arch->getComponentRaw(entity, component.bitmask);
	}

	void* GameObject::getComponentRaw(const std::size_t bitmask) const
	{
		return arch->getComponentRaw(entity, bitmask);
	}
}