#include <iostream>
#include <memory>
#include <mutex>
#include "Events/event.h"
#include "event_manager.h"
#include "Core/logger.h"

namespace Lynx {

#ifdef LYNX_MULTITHREAD
std::mutex EventManager::queue_mutex;
std::queue<Event> EventManager::event_queue;
#endif

std::map<EventType, std::vector<EventListener>> EventManager::listeners = std::map<EventType, std::vector<EventListener>>();
unsigned int EventManager::last_ev_id = 0;

EventListener EventManager::AddListener(const EventType& type, EventCallbackFunc&& func)
{
	EventListener n_listener = EventListener(func, last_ev_id++);
	listeners[type].push_back(n_listener);
	return n_listener;
}

void EventManager::RemoveListener(const EventType& type, EventListener listener)
{
	auto p = std::find(listeners[type].begin(), listeners[type].end(), listener);

	if(p == listeners[type].end())
		return;

	listeners[type].erase(p);
}

void EventManager::SendEvent(const Event& event)
{
#ifdef LYNX_MULTITHREAD
	std::unique_lock<std::mutex> lock(queue_mutex);
	event_queue.push(event);
#else
	EventType type = event.GetType();

	if(listeners.find(type) == listeners.end())
		return;

	for(int i = 0; i < listeners[type].size(); i++){
		auto listener = listeners[type][i];

		listener(event);
	}
#endif
}

#ifdef LYNX_MULTITHREAD
void EventManager::UpdateListeners()
{
	std::unique_lock<std::mutex> lock(queue_mutex);

	if(event_queue.empty())
		return;


	Event event = event_queue.back();
	EventType type = event.GetType();

	if(listeners.find(type) == listeners.end())
		return;

	for(int i = 0; i < listeners[type].size(); i++){
		auto listener = listeners[type][i];

		listener(event);
	}

	event_queue.pop();
}
#endif

}
