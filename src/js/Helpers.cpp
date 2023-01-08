#include "pch.hpp"
#include "Concepts.hpp"
#include "js/Env.hpp"
#include "js/Helpers.hpp"

namespace DarkDescent::JS
{
	v8::Local<v8::Number> number(const Env& env, uint8_t number) { return v8::Number::New(env.isolate(), number); }
	v8::Local<v8::Number> number(const Env& env, uint16_t number) { return v8::Number::New(env.isolate(), number); }
	v8::Local<v8::Number> number(const Env& env, uint32_t number) { return v8::Number::New(env.isolate(), number); }
	v8::Local<v8::BigInt> number(const Env& env, uint64_t number) { return v8::BigInt::New(env.isolate(), number); }
	v8::Local<v8::Number> number(const Env& env, int8_t number) { return v8::Number::New(env.isolate(), number); }
	v8::Local<v8::Number> number(const Env& env, int16_t number) { return v8::Number::New(env.isolate(), number); }
	v8::Local<v8::Number> number(const Env& env, int32_t number) { return v8::Number::New(env.isolate(), number); }
	v8::Local<v8::BigInt> number(const Env& env, int64_t number) { return v8::BigInt::New(env.isolate(), number); }
	v8::Local<v8::Number> number(const Env& env, float number) { return v8::Number::New(env.isolate(), number); }
	v8::Local<v8::Number> number(const Env& env, double number) { return v8::Number::New(env.isolate(), number); }

	v8::Local<v8::String> string(const Env& env, const char* str) { return v8::String::NewFromUtf8(env.isolate(), str).ToLocalChecked(); }
	v8::Local<v8::String> string(const Env& env, const std::string& str) { return v8::String::NewFromUtf8(env.isolate(), str.c_str()).ToLocalChecked(); }
	v8::Local<v8::String> string(const Env& env, std::string&& str) { return v8::String::NewFromUtf8(env.isolate(), str.c_str()).ToLocalChecked(); }

	std::string parseString(const Env& env, v8::Local<v8::Value> val)
	{
		v8::String::Utf8Value strVal(env.isolate(), val->ToString(env.context()).ToLocalChecked());
		return std::string(*strVal);
	}

	bool parseString(const Env& env, v8::Local<v8::Value> val, std::string& target)
	{
		v8::Local<v8::String> s;
		if (val->ToString(env.context()).ToLocal(&s))
		{
			v8::String::Utf8Value strVal(env.isolate(), s);
			target = std::string(*strVal);
			return true;
		}
		return false;
	}

	bool getFromObject(const Env& env, v8::Local<v8::Value> val, const char* key, v8::Local<v8::Value>& out)
	{
		v8::Local<v8::Object> obj;
		if (val->ToObject(env.context()).ToLocal(&obj))
			return obj->Get(env.context(), string(env, key)).ToLocal(std::addressof(out));
		return false;
	}

	bool getFromObject(const Env& env, v8::Local<v8::Value> val, v8::Local<v8::Value> key, v8::Local<v8::Value>& out)
	{
		v8::Local<v8::Object> obj;
		if (val->ToObject(env.context()).ToLocal(&obj))
			return obj->Get(env.context(), key).ToLocal(std::addressof(out));
		return false;
	}

	v8::MaybeLocal<v8::Value> getFromObject(const Env& env, v8::Local<v8::Value> val, const char* key)
	{
		v8::Local<v8::Object> obj;
		if (val->ToObject(env.context()).ToLocal(&obj))
			return obj->Get(env.context(), string(env, key));
		return v8::MaybeLocal<v8::Value>();
	}

	v8::MaybeLocal<v8::Value> getFromObject(const Env& env, v8::Local<v8::Value> val, v8::Local<v8::Value> key)
	{
		v8::Local<v8::Object> obj;
		if (val->ToObject(env.context()).ToLocal(&obj))
			return obj->Get(env.context(), key);
		return v8::MaybeLocal<v8::Value>();
	}
	
	v8::Local<v8::Array> mapStringArray(const Env& env, std::vector<const char*>& vec)
	{
		const std::size_t l = vec.size();
		v8::Local<v8::Array> arr = v8::Array::New(env.isolate(), static_cast<uint32_t>(l));
		for(std::size_t i = 0; i < l; i++)
			arr->Set(env.context(), static_cast<uint32_t>(i), string(env, vec.at(i)));
		return arr;
	}

	v8::Local<v8::Array> mapStringArray(const Env& env, std::vector<std::string>& vec)
	{
		const std::size_t l = vec.size();
		v8::Local<v8::Array> arr = v8::Array::New(env.isolate(), static_cast<uint32_t>(l));
		for(std::size_t i = 0; i < l; i++)
			arr->Set(env.context(), static_cast<uint32_t>(i), string(env, vec.at(i)));
		return arr;
	}

	void setInternalPointer(const v8::FunctionCallbackInfo<v8::Value> &args, void* pointer, uint32_t i)
	{
		args.This()->SetInternalField(i, v8::External::New(args.GetIsolate(), pointer));
	}

	void setInternalPointer(const Env& env, v8::Local<v8::Value> val, void* pointer, uint32_t index)
	{
		val.As<v8::Object>()->SetInternalField(index, v8::External::New(env.isolate(), pointer));
	}

	v8::MaybeLocal<v8::Value> callConstructor(const Env& env, v8::Local<v8::Value> value, std::vector<v8::Local<v8::Value>>& args)
	{
		return value.As<v8::Function>()->CallAsConstructor(env.context(), static_cast<int>(args.size()), &args.at(0));
	}

	v8::MaybeLocal<v8::Value> callConstructor(const Env& env, v8::Local<v8::Value> value, std::vector<v8::Local<v8::Value>>&& args)
	{
		return value.As<v8::Function>()->CallAsConstructor(env.context(), static_cast<int>(args.size()), &args.at(0));
	}
}