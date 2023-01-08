#include "pch.hpp"
#include "js/Env.hpp"
#include "js/Object.hpp"
#include "js/Helpers.hpp"

namespace DarkDescent::JS
{
	Object::Object(const Env& env, v8::Local<v8::Value> obj): Object(env, obj.IsEmpty() ? v8::Object::New(env.isolate()) : obj->ToObject(env.context()).ToLocalChecked()) { }
	Object::Object(const Env& env, v8::Local<v8::Object> obj): env(env), v8Object_(obj.IsEmpty() ? v8::Object::New(env.isolate()) : obj) { }

	v8::MaybeLocal<v8::Value> Object::operator [](const char* key) const
	{
		return v8Object_->Get(env.context(), v8::String::NewFromUtf8(env.isolate(), key).ToLocalChecked());
	}

	v8::MaybeLocal<v8::Value> Object::operator [](const std::string& key) const
	{
		return this->operator[](key.c_str());
	}

	v8::MaybeLocal<v8::Value> Object::operator [](std::string&& key) const
	{
		return this->operator[](key.c_str());
	}

	v8::MaybeLocal<v8::Value> Object::getPrivate(const char* key) const
	{
		return v8Object_->GetPrivate(env.context(), v8::Private::New(env.isolate(), JS::string(env, key)));
	}

	bool Object::set(const char* key, v8::Local<v8::Value> val, v8::PropertyAttribute attributes) const
	{
		return v8Object_->DefineOwnProperty(env.context(), v8::String::NewFromUtf8(env.isolate(), key).ToLocalChecked(), val, attributes).FromJust();
	}

	bool Object::set(v8::Local<v8::String> name, v8::Local<v8::Value> val, v8::PropertyAttribute attributes) const
	{
		return v8Object_->DefineOwnProperty(env.context(), name, val, attributes).FromJust();
	}

	bool Object::set(const char* name, v8::FunctionCallback callback, v8::Local<v8::Value> data, v8::PropertyAttribute attributes) const
	{
		return v8Object_->DefineOwnProperty(env.context(), v8::String::NewFromUtf8(env.isolate(), name).ToLocalChecked(), v8::FunctionTemplate::New(env.isolate(), callback, data)->GetFunction(env.context()).ToLocalChecked(), attributes).FromJust();
	}

	bool Object::set(const char* name, v8::FunctionCallback callback, void* data, v8::PropertyAttribute attributes) const
	{
		return v8Object_->DefineOwnProperty(env.context(), v8::String::NewFromUtf8(env.isolate(), name).ToLocalChecked(), v8::FunctionTemplate::New(env.isolate(), callback, v8::External::New(env.isolate(), data))->GetFunction(env.context()).ToLocalChecked(), attributes).FromJust();
	}

	bool Object::set(v8::Local<v8::String> name, v8::FunctionCallback callback, v8::Local<v8::Value> data, v8::PropertyAttribute attributes) const
	{
		return v8Object_->DefineOwnProperty(env.context(), name, v8::FunctionTemplate::New(env.isolate(), callback, data)->GetFunction(env.context()).ToLocalChecked(), attributes).FromJust();
	}

	bool Object::set(v8::Local<v8::String> name, v8::FunctionCallback callback, void* data, v8::PropertyAttribute attributes) const
	{
		return v8Object_->DefineOwnProperty(env.context(), name, v8::FunctionTemplate::New(env.isolate(), callback, v8::External::New(env.isolate(), data))->GetFunction(env.context()).ToLocalChecked(), attributes).FromJust();
	}
};