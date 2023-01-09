#pragma once

#pragma once

#include "pch.hpp"
#include "Concepts.hpp"
#include "js/Env.hpp"

#define JS_CALLBACK(__NAME__) void __NAME__##wrapped(const JS::Env& env, const v8::FunctionCallbackInfo<v8::Value>& args);\
void __NAME__ (const v8::FunctionCallbackInfo<v8::Value>& args) { __NAME__##wrapped(JS::Env::fromArgs(args), args); }\
void __NAME__##wrapped(const JS::Env& env, const v8::FunctionCallbackInfo<v8::Value>& args)

namespace DarkDescent::JS
{
	v8::Local<v8::Number> number(const Env& env, uint8_t number);
	v8::Local<v8::Number> number(const Env& env, uint16_t number);
	v8::Local<v8::Number> number(const Env& env, uint32_t number);
	v8::Local<v8::BigInt> number(const Env& env, uint64_t number);
	v8::Local<v8::Number> number(const Env& env, int8_t number);
	v8::Local<v8::Number> number(const Env& env, int16_t number);
	v8::Local<v8::Number> number(const Env& env, int32_t number);
	v8::Local<v8::BigInt> number(const Env& env, int64_t number);
	v8::Local<v8::Number> number(const Env& env, float number);
	v8::Local<v8::Number> number(const Env& env, double number);

	v8::Local<v8::String> string(const Env& env, const char* str);
	v8::Local<v8::String> string(const Env& env, const std::string& str);
	v8::Local<v8::String> string(const Env& env, std::string&& str);

	std::string parseString(const Env& env, v8::Local<v8::Value> val);
	bool parseString(const Env& env, v8::Local<v8::Value> val, std::string& target);

	bool getFromObject(const Env& env, v8::Local<v8::Value> val, const char* key, v8::Local<v8::Value>& out);
	bool getFromObject(const Env& env, v8::Local<v8::Value> val,  v8::Local<v8::Value> key, v8::Local<v8::Value>& out);
	v8::MaybeLocal<v8::Value> getFromObject(const Env& env, v8::Local<v8::Value> val, const char* key);
	v8::MaybeLocal<v8::Value> getFromObject(const Env& env, v8::Local<v8::Value> val,  v8::Local<v8::Value> key);

	template<typename T>
	bool getFromObject(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val, v8::Local<v8::Value> key, v8::Local<T>& out)
	{
		v8::Local<v8::Object> obj;
		if (val->ToObject(ctx).ToLocal(&obj))
		{
			v8::Local<v8::Value> val;
			if(obj->Get(ctx, key).ToLocal(&val))
			{
				out = val.As<T>();
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool getFromObject(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val, const char* key, v8::Local<T>& out)
	{
		v8::Local<v8::Object> obj;
		if (val->ToObject(ctx).ToLocal(&obj))
		{
			v8::Local<v8::Value> val;
			if(obj->Get(ctx, v8::String::NewFromUtf8(ctx->GetIsolate(), key).ToLocalChecked()).ToLocal(&val))
			{
				out = val.As<T>();
				return true;
			}
		}
		return false;
	}

	v8::Local<v8::Array> mapStringArray(const Env& env, std::vector<const char*>& vec);
	v8::Local<v8::Array> mapStringArray(const Env& env, std::vector<std::string>& vec);

	template<typename T>
	T* getInternalPointer(const v8::FunctionCallbackInfo<v8::Value> &args, uint32_t i = 0)
	{
		return static_cast<T*>(args.This()->GetInternalField(i).As<v8::External>()->Value());
	}

	template<typename T>
	T* getInternalPointer(const v8::Local<v8::Value> &obj, uint32_t i = 0)
	{
		return static_cast<T*>(obj.As<v8::Object>()->GetInternalField(i).As<v8::External>()->Value());
	}

	void setInternalPointer(const v8::FunctionCallbackInfo<v8::Value> &args, void* pointer, uint32_t index = 0);
	void setInternalPointer(const Env& env, v8::Local<v8::Value> val, void* pointer, uint32_t index = 0);

	template<typename T>
	T* parseExternal(const Env& env, v8::Local<v8::Value> val) { return val->IsExternal() ? static_cast<T*>(val.As<v8::External>()->Value()) : nullptr; }

	template<typename T>
	T* parseExternalData(const v8::FunctionCallbackInfo<v8::Value> &args) { return parseExternal<T>(JS::Env::fromArgs(args), args.Data()); }
	
	template<typename Index>
	void setInternal(v8::Local<v8::Value> val, Index index, v8::Local<v8::Value> value) { val.As<v8::Object>()->SetInternalField(index, value); }

	template<typename T>
	bool parseNumber(const JS::Env& env, v8::Local<v8::Value> val, T& out)
	{
		if(val.IsEmpty())
			return false;
			
		if(val->IsNumber())
		{
			out = static_cast<T>(val.As<v8::Number>()->IntegerValue(env.context()).ToChecked());
			return true;
		}
		else if(val->IsBigInt())
		{
			out = static_cast<T>(val.As<v8::BigInt>()->Uint64Value());
			return true;
		}
		return false;
	}

	v8::MaybeLocal<v8::Value> callConstructor(const Env& env, v8::Local<v8::Value> value, std::vector<v8::Local<v8::Value>>& args);
	v8::MaybeLocal<v8::Value> callConstructor(const Env& env, v8::Local<v8::Value> value, std::vector<v8::Local<v8::Value>>&& args);
}