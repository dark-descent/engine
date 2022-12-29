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

	private:
		void initialize();
		void allInitialized();
		void terminate();
	
	protected:
		virtual void onInitialize();
		virtual void onReady();
		virtual void onTerminate();

		void emitEvent(const char* name, void* data = nullptr);
		void addEventHandler(const char* name, EventHandler eventHandler, void* data = nullptr);
		void removeEventHandler(const char* name, EventHandler eventHandler);

		void emitEvent(Hash name, void* data = nullptr);
		void addEventHandler(Hash name, EventHandler eventHandler, void* data = nullptr);
		void removeEventHandler(Hash name, EventHandler eventHandler);

		template<typename T>
		void emitEvent(T event, void* data = nullptr)
		{
			emitEvent(static_cast<Hash>(event), data);
		}

		template<typename T>
		void addEventHandler(T event, EventHandler eventHandler, void* data = nullptr)
		{
			addEventHandler(static_cast<Hash>(event), eventHandler, data);
		}

	private:
		std::string name_;

	protected:
		const Engine& engine_;

		friend class Engine;
	};

	template <typename T>
	concept IsSubSystem = std::is_base_of<SubSystem, T>::value;
}