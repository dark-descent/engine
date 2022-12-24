#pragma once

#include "pch.hpp"
#include "Hash.hpp"

namespace DarkDescent
{
	class Engine;
	class SubSystem;

	struct Event
	{
		SubSystem* system;
		const void* data;
	};

	using EventHandler = void(*)(SubSystem* self, const Event&, const void* data);

	struct EventHandlerInfo
	{
		SubSystem* owner;
		EventHandler handler;
		const void* data;
	};

	class EventManager
	{
	public:
		EventManager();
		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		~EventManager();

		void emit(Hash hash, const Event& event) const;
		void on(Hash hash, SubSystem* owner, EventHandler handler, const void* data = nullptr) const;
		void remove(Hash hash, EventHandler handler) const;
		
		mutable std::unordered_map<Hash, std::vector<EventHandlerInfo>> handlers_;
	};
}