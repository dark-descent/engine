#include "js/GameObject.hpp"
#include "Engine.hpp"
#include "SceneManager.hpp"
#include "js/Helpers.hpp"
#include "GameObject.hpp"

namespace DarkDescent::JS
{
	JS_CLASS_METHOD_IMPL(GameObjectClass::ctor)
	{
		const std::size_t l = args.Length();
		if (l == 1)
		{
			if (args[0]->IsExternal())
			{
				args.This()->SetInternalField(0, args[0]);
				return;
			}
		}

		SceneManager* sm = JS::parseExternalData<SceneManager>(args);
		if (sm->loadingScene().has_value())
		{
			args.This()->SetInternalField(0, v8::External::New(env.isolate(), sm->loadingScene().value()->spawn()));
		}
		else if (sm->activeScene().has_value())
		{
			args.This()->SetInternalField(0, v8::External::New(env.isolate(), sm->activeScene().value()->spawn()));
		}
		else
		{
			env.throwException("Could not set internal field!");
		}
	}

	JS_CLASS_METHOD_IMPL(GameObjectClass::addComponent)
	{
		if (args.Length() >= 1)
		{
			if (args[0]->IsFunction())
			{
				v8::Local<v8::Value> ctorArgs[1] = { args.This()->GetInternalField(0) };
				v8::Local<v8::Value> component;
				if(JS::callConstructor(env, args[0], { args.This() }).ToLocal(&component))
				{
					return args.GetReturnValue().Set(component);
				}
			}
		}
		
		env.throwException("Could not get component!");
	}

	JS_CLASS_METHOD_IMPL(GameObjectClass::getComponent)
	{

	}

	JS_CLASS_METHOD_IMPL(GameObjectClass::removeComponent)
	{

	}

	JS_CREATE_CLASS(GameObjectClass)
	{
		builder.setInternalFieldCount(1);
		builder.setName("GameObject");
		builder.setConstructor(ctor, Engine::getInstance().getSubSystem<SceneManager>());
		builder.setMethod("addComponent", addComponent);
		builder.setMethod("getComponent", getComponent);
		builder.setMethod("removeComponent", removeComponent);
	}
}