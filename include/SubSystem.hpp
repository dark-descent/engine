#pragma once

#define SUB_SYSTEM_CTORS(CLASS_NAME) friend class Engine; \
CLASS_NAME(const CLASS_NAME&) = delete; \
CLASS_NAME(CLASS_NAME&&) = delete; \
virtual ~CLASS_NAME() { } \
CLASS_NAME(const char* name, const Engine& engine): SubSystem(name, engine)

#include "EventManager.hpp"

namespace DarkDescent
{
	class Engine;

	class SubSystem
	{
	protected:
		SubSystem(const char* name, const Engine& engine);
		SubSystem(const SubSystem&) = delete;
		SubSystem(SubSystem&&) = delete;
		virtual ~SubSystem() = 0;

	protected:
		void initialize();
		void terminate();

		virtual void onInitialize();
		virtual void onTerminate();

		void emitEvent(const char* name, const void* data = nullptr);
		void addEventHandler(const char* name, EventHandler eventHandler, const void* data = nullptr);
		void removeEventHandler(const char* name, EventHandler eventHandler);

		void emitEvent(Hash name, const void* data = nullptr);
		void addEventHandler(Hash name, EventHandler eventHandler, const void* data = nullptr);
		void removeEventHandler(Hash name, EventHandler eventHandler);

	private:
		std::string name_;
	
	protected:
		const Engine& engine_;

		friend class Engine;
	};

	template <typename T>
	concept IsSubSystem = std::is_base_of<SubSystem, T>::value;
}