#pragma once

#include "pch.hpp"

namespace DarkDescent::JS
{
	class Env;

	class Object
	{
	public:
		Object(const Env& env, v8::Local<v8::Value> obj = v8::Local<v8::Object>());
		Object(const Env& env, v8::Local<v8::Object> obj);

		v8::Local<v8::Object> operator*() const { return v8Object_; }
		v8::Local<v8::Object> operator->() const { return v8Object_; }

		v8::MaybeLocal<v8::Value> operator [](const char* key) const;
		v8::MaybeLocal<v8::Value> operator [](const std::string& key) const;
		v8::MaybeLocal<v8::Value> operator [](std::string&& key) const;

		v8::MaybeLocal<v8::Value> getPrivate(const char* key) const;

		bool set(const char* key, v8::Local<v8::Value> val, v8::PropertyAttribute attributes = v8::None) const;
		bool set(v8::Local<v8::String> name,  v8::Local<v8::Value> val, v8::PropertyAttribute attributes = v8::None) const;

		bool set(const char* name, v8::FunctionCallback callback, v8::Local<v8::Value> data = v8::Local<v8::Value>(), v8::PropertyAttribute attributes = v8::None) const;
		bool set(const char* name, v8::FunctionCallback callback, void* data, v8::PropertyAttribute attributes = v8::None) const;
		bool set(v8::Local<v8::String> name, v8::FunctionCallback callback, v8::Local<v8::Value> data, v8::PropertyAttribute attributes = v8::None) const;
		bool set(v8::Local<v8::String> name, v8::FunctionCallback callback, void* data, v8::PropertyAttribute attributes = v8::None) const;

	public:
		const Env& env;

	private:
		v8::Local<v8::Object> v8Object_;
	};
};