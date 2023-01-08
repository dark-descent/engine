#pragma once

#include "js/GameObject.hpp"

namespace DarkDescent::JS
{
	JS_CLASS_METHOD_IMPL(GameObjectClass::ctor)
	{

	}
	
	JS_CLASS_METHOD_IMPL(GameObjectClass::addComponent)
	{

	}

	JS_CLASS_METHOD_IMPL(GameObjectClass::getComponent)
	{

	}
	
	JS_CLASS_METHOD_IMPL(GameObjectClass::removeComponent)
	{

	}
	
	JS_CREATE_CLASS(GameObjectClass)
	{
		builder.setName("GameObject");
		builder.setConstructor(ctor);
		builder.setMethod("addComponent", addComponent);
		builder.setMethod("getComponent", getComponent);
		builder.setMethod("removeComponent", removeComponent);
	}
}