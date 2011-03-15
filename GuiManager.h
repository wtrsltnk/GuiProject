/*
 * GuiManager.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef GUIMANAGER_H_
#define GUIMANAGER_H_

#include "events.h"
#include <vector>

namespace ui
{

class EventHandler;
class Font;

class GuiManager
{
	GuiManager();
	static GuiManager* sInstance;
public:
	static GuiManager* createInstance(const char* fontpath);
	static GuiManager* instance();
	static void destroyInstance();
	virtual ~GuiManager();

	void setupSize(int w, int h);
	void onKeyboard(unsigned char key, int x, int y);
	void onSpecialKeyboard(int key, int x, int y);
	void onMouseClick(int button, int state, int x, int y);
	void onMouseMove(int x, int y);
	void render();

private:
	void addEventHandler(GuiEventHandler* handler, eventFn method, Control* box, int eventType);
	void removeEventHandler(GuiEventHandler* handler, eventFn method, Control* box);
	void initiateEvent(Control* box, int eventType, EventArgs* e);

	// These two are only needed by the Control class in its constructor and destructor
	void addControl(Control* ctr);
	void removeControl(Control* ctr);

private:
	static Font* sDefaultFont;
	int mViewSize[2];
	std::vector<EventHandler*> mHandlers;
	std::vector<Control*> mControls;
	Control* mFocus;

	friend class Control;
	template <int t> friend class Event;

#ifdef USE_GLUT
public:
	static void glutKeyboard(unsigned char key, int x, int y);
	static void glutSpecialKeyboard(int key, int x, int y);
	static void glutMouseClick(int button, int state, int x, int y);
	static void glutMouseMove(int x, int y);
#endif
};

template <int t>
class Event
{
public:
	class Handler
	{
	public:
		Handler(GuiEventHandler* handler, eventFn method) : mHandler(handler), mMethod(method), mEventType(t) { }

		inline void operator () (Control* ctr, EventArgs* e)
		{ ((this->mHandler)->*this->mMethod)(ctr, e); }

	private:
		GuiEventHandler* mHandler;
		eventFn mMethod;
		int mEventType;

		friend class Event;
	};
public:
	Event(Control* ctr) : mControl(ctr) { }

	inline void operator += (Handler handler)
			{ GuiManager::instance()->addEventHandler(handler.mHandler, handler.mMethod, this->mControl, handler.mEventType); }

	inline void operator -= (Handler handler)
			{ GuiManager::instance()->removeEventHandler(handler.mHandler, handler.mMethod, this->mControl); }

	inline void operator () (EventArgs* e)
			{ GuiManager::instance()->initiateEvent(this->mControl, t, e); }
private:
	Control* mControl;

};

}

#endif /* GUIMANAGER_H_ */
