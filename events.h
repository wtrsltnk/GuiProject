/*
 * events.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include <list>

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

template <class S, class E>
class Event
{
public:
typedef void (EventHandler::*FunctionPtr)(S* sender, EventArgs*);

	class Handler
	{
	public:
		Handler(EventHandler* handler, FunctionPtr method) : mHandler(handler), mMethod(method) { }

		inline void operator () (S* sender, E* e) { ((this->mHandler)->*this->mMethod)(sender, e); }

	private:
		EventHandler* mHandler;
		FunctionPtr mMethod;

		friend class Event;
	};

public:
	Event(S* sender) : mSender(sender) { }

	inline void operator += (Handler handler)
	{
		this->mHandlers.push_back(handler);
	}

	inline void operator -= (Handler handler)
	{
		this->mHandlers.remove(handler);
	}

	inline void operator () (E* e)
	{
		typename std::list<Handler>::iterator itr;
		for (itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
			(*itr)(this->mSender, e);
	}

private:
	S* mSender;
	typename std::list<Handler> mHandlers;

};

}

#endif /* EVENTS_H_ */
