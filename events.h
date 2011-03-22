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

// A class that wants to handle events needs to inherrit from this class
class EventHandler
{
public:
	virtual ~EventHandler() { }
};

// A base class for Event arguments
class EventArgs
{
public:
	virtual ~EventArgs() { }
};

// The Event class to which you can add handlers. When firing this Event, all the handlers will be called.
template <class S, class E>
class Event
{
public:
	// Type definition for the Member Function Pointer
	typedef void (EventHandler::*FunctionPtr)(S* sender, EventArgs*);

public:
	// This Handler class holds the data for an event call.
	class Handler
	{
	public:
		Handler(EventHandler* handler, FunctionPtr method) : mHandler(handler), mMethod(method) { }

		// Fire this handler with given sender and event args
		inline void operator () (S* sender, E* e) { ((this->mHandler)->*this->mMethod)(sender, e); }

	private:
		EventHandler* mHandler;
		FunctionPtr mMethod;

		// Make sure Event class can access the private members of this Handler class
		friend class Event;
	};

public:
	Event(S* sender) : mSender(sender) { }

	// Add the given handler to this event
	inline void operator += (Handler handler) { this->mHandlers.push_back(handler); }

	// Remove the given handler from this event
	inline void operator -= (Handler handler)
	{
		for (typename std::vector<Handler>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
		{
			if ((*itr).mHandler == handler.mHandler && (*itr).mMethod == handler.mMethod)
			{
				this->mHandlers.erase(itr);
				break;
			}
		}
	}

	// Fire this event with the given event arguments
	inline void operator () (E* e)
	{
		for (typename std::vector<Handler>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
			(*itr)(this->mSender, e);
	}

private:
	S* mSender;
	typename std::vector<Handler> mHandlers;

};

}

#endif /* EVENTS_H_ */
