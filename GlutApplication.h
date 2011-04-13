/*
 * GlutApplication.h
 *
 *  Created on: 15 feb 2011
 *      Author: 3336727
 */

#ifndef GLUTAPPLICATION_H_
#define GLUTAPPLICATION_H_

#include "userInput.h"

class GlutApplication
{
public:
	GlutApplication(const char* title);
	virtual ~GlutApplication() { }

	void quit();

	virtual bool initialize(int argc, char* argv[]) { return true; }
	virtual void resize(int w, int h) { }
	virtual void onKeyDown(Key::Code key) { }
	virtual void onKeyUp(Key::Code key) { }
	virtual void onMouseButtonDown(Mouse::Button button) { }
	virtual void onMouseButtonUp(Mouse::Button button) { }
	virtual void onMouseMove(int x, int y) { }
	virtual void render(int time) { }

	int width, height;
	char title[64];

};

extern GlutApplication* gApplication;

/* Example class:
 *
	class Example01 : public GlutApplication
	{
	public:
		Example01();
		virtual ~Example01();

		virtual bool initialize(int argc, char* argv[]) { return true; }
		virtual void resize(int w, int h) { }
		virtual void onKeyDown(Key::Code key) { }
		virtual void onKeyUp(Key::Code key) { }
		virtual void onMouseButtonDown(Mouse::Button button) { }
		virtual void onMouseButtonUp(Mouse::Button button) { }
		virtual void onMouseMove(int x, int y) { }
		virtual void render(int time) { }
	};
 *
 * Don't forget to include the following line somewhere
 * in your implementation (cpp) file:
 *
 * 		GlutApplication* gApplication = new Example01();
 *
 */

#endif /* GLUTAPPLICATION_H_ */
