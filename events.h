/*
 * events.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef EVENTS_H_
#define EVENTS_H_

namespace ui
{

class Control;

class GuiEventHandler
{
public:
	virtual ~GuiEventHandler() { }
};

class EventArgs
{
public:
	virtual ~EventArgs() { }
};

typedef void (GuiEventHandler::*eventFn)(Control*, EventArgs*);

namespace EventType
{
enum
{
	Click = 0,
	StateChanged,
	TextChanged,
};
}

}

#endif /* EVENTS_H_ */
