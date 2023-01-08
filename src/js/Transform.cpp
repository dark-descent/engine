#include "js/Transform.hpp"

namespace DarkDescent::JS
{
	JS_CLASS_METHOD_IMPL(TransformClass::ctor)
	{

	}
	
	JS_CREATE_CLASS(TransformClass)
	{
		builder.setName("Transform");
		builder.setConstructor(ctor);
	}
}