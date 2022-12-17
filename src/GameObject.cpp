#include "GameObject.hpp"

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
}