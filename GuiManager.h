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

class VerticalContainer;
class Font;

class GuiManager : public EventManager<ui::Control>
{
	GuiManager();
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

	VerticalContainer* getRoot();
	Control* getTopControlAt(float point[2], VerticalContainer* container = 0);

private:
	virtual void initialize(const char* fontpath);
	virtual void addEventHandler(EventHandler* handler, eventFn method, Control* box, int eventType);
	virtual void removeEventHandler(EventHandler* handler, eventFn method, Control* box);
	virtual void initiateEvent(Control* box, int eventType, EventArgs* e);

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

#endif /* GUIMANAGER_H_ */
