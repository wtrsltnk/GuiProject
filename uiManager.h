/*
 * uiManager.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "events.h"
#include <vector>

namespace ui
{

class Control;
class VerticalContainer;
class Font;

class Manager : public event::EventManager<Control>
{
	Manager();
public:
	static Manager* createInstance(const char* fontpath);
	static Manager* instance();
	static void destroyInstance();
	virtual ~Manager();

	void setupSize(int w, int h);
	void onKeyboard(unsigned char key, int x, int y);
	void onSpecialKeyboard(int key, int x, int y);
	void onMouseClick(int button, int state, int x, int y);
	void onMouseMove(int x, int y);
	void render();

	VerticalContainer* getRoot();
	Control* getTopControlAt(float point[2], VerticalContainer* container = 0);

private:
	virtual void initialize(const char* fontpath);
	virtual void addEventHandler(event::EventHandler* handler, event::EventFunctionPtr method, Control* box, int eventType);
	virtual void removeEventHandler(event::EventHandler* handler, event::EventFunctionPtr method, Control* box);
	virtual void initiateEvent(Control* box, int eventType, event::EventArgs* e);

private:
	void addControl(Control* ctr);
	void removeControl(Control* ctr);
	friend class Control;

private:
	static Font* sDefaultFont;
	int mViewSize[2];
	std::vector<Control*> mControls;
	Control* mFocus;
	VerticalContainer* mRoot;

#ifndef SKIP_GLUT
public:
	static void glutKeyboardDown(unsigned char key, int x, int y);
	static void glutKeyboardUp(unsigned char key, int x, int y);
	static void glutSpecialKeyboardDown(int key, int x, int y);
	static void glutSpecialKeyboardUp(int key, int x, int y);
	static void glutMouseClick(int button, int state, int x, int y);
	static void glutMouseMove(int x, int y);
#endif
};

}

#endif /* UIMANAGER_H_ */
