#pragma once

namespace DarkDescent::JS::Component
{
	GameObject* parseCtorArg(const v8::FunctionCallbackInfo<v8::Value>& args);
}