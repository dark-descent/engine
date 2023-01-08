#include "js/Transform.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"
namespace DarkDescent::JS
{
	JS_CLASS_METHOD_IMPL(TransformClass::ctor)
	{
		if(args.Length() >= 1 && args[0]->IsObject())
		{
			auto go = static_cast<GameObject*>(args[0].As<v8::Object>()->GetInternalField(0).As<v8::External>()->Value());
			auto t = go->addComponent<Transform>();
			Logger::get().debug("Added component ", t->log());
		}
		else
		{

		}
	}
	
	JS_CREATE_CLASS(TransformClass)
	{
		builder.setName("Transform");
		builder.setConstructor(ctor);
	}
}