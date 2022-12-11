#include "js/Env.hpp"
#include "Logger.hpp"

namespace DarkDescent::JS
{
	Env Env::create()
	{
		using namespace v8;
		v8::Isolate::CreateParams params;
		params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		return Env(std::move(params));
	}

	Env* Env::createNew()
	{
		using namespace v8;
		v8::Isolate::CreateParams params;
		params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		return new Env(std::move(params));
	}

	Env::Env(v8::Isolate::CreateParams&& createParams):
		createParams_(createParams),
		isolate_(v8::Isolate::New(createParams_))
	{
		v8::Isolate::Scope isolateScope(isolate_);
		v8::HandleScope handleScope(isolate_);
		context_.Reset(isolate_, v8::Context::New(isolate_));
	}

	Env::~Env()
	{
		Logger::get().info("Terminating JS::Env...");
		context_.Reset();
		isolate_->Dispose();
		delete createParams_.array_buffer_allocator;
	}

	v8::MaybeLocal<v8::Value> Env::readJson(const std::filesystem::path& path)
	{
		std::ifstream is(path, std::ifstream::beg | std::ifstream::binary);
		std::ostringstream ss;
		ss << is.rdbuf();
		const std::string& s = ss.str();
		v8::Local<v8::String> jsonString = v8::String::NewFromUtf8(isolate_, s.data()).ToLocalChecked();
		return v8::JSON::Parse(context(), jsonString);
	}
}