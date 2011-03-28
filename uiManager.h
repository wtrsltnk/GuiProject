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
class Container;
class Font;

class Manager
{
	Manager();
	static Manager* sInstance;
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

	Container* getRoot();
	Control* getTopControlAt(float point[2], Container* container = 0);

private:
	virtual void initialize(const char* fontpath);

private:
	void addControl(Control* ctr);
	void removeControl(Control* ctr);
	friend class Control;

private:
	static Font* sDefaultFont;
	int mViewSize[2];
	std::vector<Control*> mControls;
	Control* mFocus;
	Container* mRoot;

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
