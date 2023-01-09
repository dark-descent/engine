#include "js/Material.hpp"

namespace DarkDescent::JS
{
	JS_CLASS_METHOD_IMPL(MaterialClass::ctor)
	{
		if(args.Length() >= 1)
		{
			if(args[0]->IsExternal())
			{
				JS::setInternal(args.This(), 0, args[0]);
			}
		}
		else
		{
			env.throwException("Could not create material!");
		}
	}

	JS_CLASS_METHOD_IMPL(MaterialClass::clone)
	{
		MaterialClass* jsClass = JS::parseExternalData<MaterialClass>(args);
	}
	
	JS_CLASS_METHOD_IMPL(MaterialClass::set)
	{
		MaterialClass* jsClass = JS::parseExternalData<MaterialClass>(args);
	}

	JS_CLASS_METHOD_IMPL(MaterialClass::get)
	{
		MaterialClass* jsClass = JS::parseExternalData<MaterialClass>(args);
	}

	JS_CREATE_CLASS(MaterialClass)
	{
		builder.setInternalFieldCount(1);
		builder.setName("Material");
		builder.setConstructor(ctor);
		builder.setStaticMethod("clone", clone, this);
		builder.setStaticMethod("set", set);
		builder.setStaticMethod("get", get);
	}
}