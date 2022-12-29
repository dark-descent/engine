#include "EventManager.hpp"
#include "Engine.hpp"

namespace DarkDescent
{
	EventManager::EventManager()
	{

	}

	EventManager::~EventManager()
	{

	}

	void EventManager::emit(Hash hash, const Event& event) const
	{
		if (handlers_.contains(hash))
		{
			for (const auto& info : handlers_.at(hash))
			{
				info.handler(info.owner, event, info.data);
			}
		}
	}

	void EventManager::on(Hash hash, SubSystem* owner, EventHandler handler, void* data) const
	{
		if (!handlers_.contains(hash))
		{
			handlers_.emplace(std::piecewise_construct, std::forward_as_tuple(hash), std::forward_as_tuple<std::initializer_list<EventHandlerInfo>>({ { owner, handler, data } }));
		}
		else
		{
			handlers_.at(hash).emplace_back(owner, handler, data);
		}
	}

	void EventManager::remove(Hash hash, EventHandler handler) const
	{
		if (handlers_.contains(hash))
		{
			auto& handlers = handlers_.at(hash);
			handlers.erase(std::find_if(handlers.begin(), handlers.end(), [&](const EventHandlerInfo& info) { return info.handler == handler; }));
		}
	}
}