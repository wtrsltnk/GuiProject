/*
 * uiManager.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "../events.h"
#include "../userInput.h"
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
	void render();

	Container* getRoot();
	Control* getTopControlAt(float point[2], Container* container = 0);

private:
	virtual void initialize(const char* fontpath);
	virtual void initialize(Font* font);

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

public:
	static void onKeyDown(Key::Code key);
	static void onKeyUp(Key::Code key);
	static void onMouseButtonDown(Mouse::Button button);
	static void onMouseButtonUp(Mouse::Button button);
	static void onMouseMove(int x, int y);

};

}

#endif /* UIMANAGER_H_ */
