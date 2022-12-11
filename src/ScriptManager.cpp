#include "ScriptManager.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	void ScriptManager::onInitialize()
	{
		using namespace v8;
		platform_ = platform::NewDefaultPlatform();
		V8::InitializePlatform(platform_.get());
		V8::Initialize();
		create_params_.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();

		mainIsolate_ = Isolate::New(create_params_);
		{
			Isolate::Scope isolate_scope(mainIsolate_);
			// Create a stack-allocated handle scope.
			HandleScope handle_scope(mainIsolate_);
			// Create a new context.
			Local<Context> context = Context::New(mainIsolate_);
			// Enter the context for compiling and running the hello world script.
			Context::Scope context_scope(context);
			// Create a string containing the JavaScript source code.
			Local<String> source = String::NewFromUtf8(mainIsolate_, "'Hello' + ', World!'", NewStringType::kNormal).ToLocalChecked();
			// Compile the source code.
			Local<Script> script = Script::Compile(context, source).ToLocalChecked();
			// Run the script to get the result.
			Local<Value> result = script->Run(context).ToLocalChecked();
			// Convert the result to an UTF8 string and print it.
			String::Utf8Value utf8(mainIsolate_, result);
			engine_.logger.info(std::string(*utf8));
		}
	}

	void ScriptManager::onTerminate()
	{
		mainIsolate_->Dispose();
		delete create_params_.array_buffer_allocator;
		v8::V8::Dispose();
		v8::V8::DisposePlatform();
		platform_.release();
	}

	void ScriptManager::run()
	{
		
	}
}