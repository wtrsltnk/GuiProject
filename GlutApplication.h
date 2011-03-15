/*
 * GlutApplication.h
 *
 *  Created on: 15 feb 2011
 *      Author: 3336727
 */

#ifndef GLUTAPPLICATION_H_
#define GLUTAPPLICATION_H_

#include <GL/freeglut.h>

class GlutApplication
{
public:
	GlutApplication(const char* title) { char* tmp = this->title; while (title[0] != '\0') { tmp[0] = title[0]; tmp++; title++; } tmp[0] = '\0'; }
	virtual ~GlutApplication() { }

	virtual bool initialize() { return true; }
	virtual void resize(int w, int h) { }
	virtual void onKeyboard(unsigned char key, int x, int y) { }
	virtual void onSpecialKeyboard(int key, int x, int y) { }
	virtual void onMouseClick(int button, int state, int x, int y) { }
	virtual void onMouseMove(int x, int y) { }
	virtual void render() { }

	int width, height;
	char title[64];
};

extern GlutApplication* gApplication;

/* Example class:
 *
 *	class Example01 : public GlutApplication
 *	{
 *	public:
 * 		Example01();
 * 		virtual ~Example01();
 *
 * 		virtual void resize(int w, int h);
 * 		virtual void onKeyboard(unsigned char key, int x, int y);
 * 		virtual void onMouseClick(int button, int state, int x, int y);
 * 		virtual void onMouseMove(int x, int y) { }
 * 		virtual void render();
 *	};
 *
 * Don't forget to include the following line somewhere
 * in your implementation (cpp) file:
 *
 * 		GlutApplication* gApplication = new Example01();
 *
 */

#endif /* GLUTAPPLICATION_H_ */
