#include "js/Env.hpp"
#include "Logger.hpp"
#include "js/Console.hpp"
#include "js/Object.hpp"
#include "Engine.hpp"
#include "js/Helpers.hpp"
#include "js/Process.hpp"
#include "js/Class.hpp"
#include "js/EngineNamespace.hpp"

namespace DarkDescent::JS
{
	Env Env::create(std::size_t index, const SubSystem::Logger& logger, ResourceManager& resourceManager)
	{
		using namespace v8;
		v8::Isolate::CreateParams params {};
		params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		return Env(std::move(params), index, logger, resourceManager);
	}

	Env* Env::createNew(std::size_t index, const SubSystem::Logger& logger, ResourceManager& resourceManager)
	{
		using namespace v8;
		v8::Isolate::CreateParams params {};
		params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		return new Env(std::move(params), index, logger, resourceManager);
	}

	Env::Env(v8::Isolate::CreateParams&& createParams, std::size_t index, const SubSystem::Logger& logger,ResourceManager& resourceManager):
		index(index),
		logger(logger),
		createParams_(createParams),
		isolate_(v8::Isolate::New(createParams_)),
		moduleLoader_(*this, resourceManager)
	{
		assert(isolate_->GetNumberOfDataSlots() != 0);

		isolate_->SetData(0, this);

		v8::Isolate::Scope isolateScope(isolate_);
		v8::HandleScope handleScope(isolate_);
		context_.Reset(isolate_, v8::Context::New(isolate_));
		auto ctx = context();
		v8::Context::Scope contextScope(ctx);

		JS::Object global(*this, ctx->Global());

		JS::Console::expose(*this, global);
		JS::Process::expose(*this, global);
	}

	Env::~Env()
	{
		logger.info("terminating JS::Env...");

		for (const auto& [_, jsClass] : classes_)
			delete jsClass;

		context_.Reset();
		isolate_->Dispose();
		delete createParams_.array_buffer_allocator;
	}

	v8::MaybeLocal<v8::Value> Env::readJson(const std::string& source) const
	{
		v8::Local<v8::String> jsonString = v8::String::NewFromUtf8(isolate_, source.data()).ToLocalChecked();
		return v8::JSON::Parse(context(), jsonString);
	}

	v8::MaybeLocal<v8::Value> Env::readJsonFile(const std::filesystem::path& path) const
	{
		std::ifstream is(path, std::ifstream::beg | std::ifstream::binary);
		std::ostringstream ss;
		ss << is.rdbuf();
		const std::string& s = ss.str();
		return readJson(s);
	}

	void Env::throwException(const char* error) const
	{
		isolate_->ThrowException(string(*this, error));
	}
}