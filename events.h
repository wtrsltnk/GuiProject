/*
 * events.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include <vector>

namespace event
{

class EventHandler
{
public:
	virtual ~EventHandler() { }
};

class EventArgs
{
public:
	virtual ~EventArgs() { }
};

typedef void (EventHandler::*EventFunctionPtr)(void* sender, EventArgs*);

template <class T>
class EventManager
{
protected:
	static EventManager* sInstance;	// This static member needs to be defined outside this header

public:
	static EventManager* instance() { return EventManager::sInstance; }
	virtual ~EventManager() { }

	virtual void initialize(const char* fontpath) = 0;
	virtual void addEventHandler(EventHandler* handler, EventFunctionPtr method, T* box, int eventType) = 0;
	virtual void removeEventHandler(EventHandler* handler, EventFunctionPtr method, T* box) = 0;
	virtual void initiateEvent(T* box, int eventType, EventArgs* e) = 0;

protected:
	class ProtectedHandler
	{
	public:
		ProtectedHandler(EventHandler* handler, EventFunctionPtr eventFn, T* control, int type) : mHandler(handler), mEventFn(eventFn), mControl(control), mEventType(type) { }

		EventHandler* mHandler;
		EventFunctionPtr mEventFn;
		T* mControl;
		int mEventType;
	};
	std::vector<ProtectedHandler*> mHandlers;

};

template <class T, class E, int t>
class Event
{
public:
	class Handler
	{
	public:
		Handler(EventHandler* handler, EventFunctionPtr method) : mHandler(handler), mMethod(method), mEventType(t) { }

		inline void operator () (T* ctr, E* e)
		{ ((this->mHandler)->*this->mMethod)(ctr, e); }

	private:
		EventHandler* mHandler;
		EventFunctionPtr mMethod;
		int mEventType;

		friend class Event;
	};

public:
	Event(T* ctr) : mControl(ctr) { }

	inline void operator += (Handler handler)
			{ EventManager<T>::instance()->addEventHandler(handler.mHandler, handler.mMethod, this->mControl, handler.mEventType); }

	inline void operator -= (Handler handler)
			{ EventManager<T>::instance()->removeEventHandler(handler.mHandler, handler.mMethod, this->mControl); }

	inline void operator () (E* e)
			{ EventManager<T>::instance()->initiateEvent(this->mControl, t, e); }
private:
	T* mControl;

};

}

#endif /* EVENTS_H_ */
