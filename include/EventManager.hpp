#pragma once

#include "pch.hpp"
#include "Hash.hpp"

#define EVENT_HANDLER() [](SubSystem* system, const Event& event, void* data)

namespace DarkDescent
{
	class Engine;
	class SubSystem;

	struct Event
	{
		SubSystem* system;
		const void* data;
	};

	using EventHandler = void(*)(SubSystem* self, const Event&, void* data);

	struct EventHandlerInfo
	{
		SubSystem* owner;
		EventHandler handler;
		void* data;
	};

	class EventManager
	{
	public:
		EventManager();
		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		~EventManager();

		void emit(Hash hash, const Event& event) const;
		void on(Hash hash, SubSystem* owner, EventHandler handler, void* data = nullptr) const;
		void remove(Hash hash, EventHandler handler) const;
		
		mutable std::unordered_map<Hash, std::vector<EventHandlerInfo>> handlers_;
	};
}