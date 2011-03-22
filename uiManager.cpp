/*
 * uiManager.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "uiManager.h"
#include "uiFont.h"
#include "uiControls.h"
#include "MainWindow.h"
#include <GL/freeglut.h>

namespace ui
{

Manager::Manager()
	: mFocus(0), mRoot(0)
{
}

Manager* Manager::sInstance = 0;
Font* Manager::sDefaultFont = 0;

Manager* Manager::createInstance(const char* fontpath)
{
	if (Manager::sInstance != 0)
		delete Manager::sInstance;

	Manager::sInstance = new Manager();
	Manager::sInstance->initialize(fontpath);

	return (Manager*)Manager::sInstance;
}

Manager* Manager::instance()
{
	return Manager::sInstance;
}

void Manager::destroyInstance()
{
	if (Manager::sInstance != 0)
		delete Manager::sInstance;
	Manager::sInstance = 0;
}

Manager::~Manager()
{
	while (this->mControls.empty() == false)
	{
		// Only delete the, since deleting automatically removes them from the GuiManager
		Control* c = this->mControls.back();
		delete c;
	}

	if (this->mRoot != 0)
		delete this->mRoot;
}

void Manager::initialize(const char* fontpath)
{
	if (Manager::sDefaultFont != 0)
		delete Manager::sDefaultFont;

	Manager::sDefaultFont = new Font();
	Manager::sDefaultFont->initializeFont(fontpath);

	Manager::instance()->mRoot = new VerticalContainer(20, 20, 10, 10);

#ifndef SKIP_GLUT
	glutKeyboardFunc(&Manager::glutKeyboardDown);
	glutKeyboardUpFunc(&Manager::glutKeyboardUp);
	glutSpecialFunc(&Manager::glutSpecialKeyboardDown);
	glutSpecialUpFunc(&Manager::glutSpecialKeyboardUp);
	glutMouseFunc(&Manager::glutMouseClick);
	glutMotionFunc(&Manager::glutMouseMove);
	glutPassiveMotionFunc(&Manager::glutMouseMove);
#endif
}

void Manager::addControl(Control* ctr)
{
	ctr->updateBox();
	this->mControls.push_back(ctr);
	if (ctr->parent() == 0 && this->mRoot != 0)
		this->mRoot->addControl(ctr);
}

void Manager::removeControl(Control* ctr)
{
	for (std::vector<Control*>::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		if ((*itr) == ctr)
		{
			this->mControls.erase(itr);
			break;
		}
	}
}

VerticalContainer* Manager::getRoot()
{
	return this->mRoot;
}

Control* Manager::getTopControlAt(float point[2], VerticalContainer* container)
{
	Control* result = 0;

	if (container == 0)
		container = Manager::instance()->mRoot;

	if (container != 0)
	{
		for (std::vector<Control*>::iterator itr = container->controls().begin(); itr != container->controls().end(); ++itr)
		{
			Control* c = (*itr);
			if (c->isPointInBox(point))
			{
				result = c;

				VerticalContainer* cc = dynamic_cast<VerticalContainer*>(c);
				if (cc != 0)
				{
					Control* tmp = getTopControlAt(point, cc);
					if (tmp != 0)
						result = tmp;
				}

				break;
			}
		}
	}

	return result;
}

void Manager::setupSize(int w, int h)
{
	this->mViewSize[0] = w;
	this->mViewSize[1] = h;
	if (this->mRoot != 0)
		this->mRoot->setSize(w-40, h-40);
}

void Manager::render()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0f, float(this->mViewSize[0]), 0.0f, float(this->mViewSize[1]), -10.0f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	// First make everything writable!
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glColorMask(0, 0, 0, 0);
	glStencilMask(0xFF);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(this->mViewSize[0], 0);
	glVertex2f(this->mViewSize[0], this->mViewSize[1]);
	glVertex2f(0, this->mViewSize[1]);
	glEnd();

	if (this->mRoot != 0)
		this->mRoot->renderControl();

	glDisable(GL_STENCIL_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

#ifndef SKIP_GLUT
void Manager::glutKeyboardDown(unsigned char key, int x, int y)
{
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->charDown(key);
	}
}
void Manager::glutKeyboardUp(unsigned char key, int x, int y)
{
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->charUp(key);
	}
}

void Manager::glutSpecialKeyboardDown(int key, int x, int y)
{
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyDown(key);
	}
}

void Manager::glutSpecialKeyboardUp(int key, int x, int y)
{
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyUp(key);
	}
}

void Manager::glutMouseClick(int button, int state, int x, int y)
{
	float point[2] = { x, Manager::instance()->mViewSize[1] - y };

	Control* control = Manager::instance()->getTopControlAt(point);

	if (control != 0)
	{
		Container* cc = 0;
		if (control->controlType() == ControlTypes::Container)
			cc = (Container*)control;
		else
			cc = control->parent();

		if (state == 0)
		{
			if (control->controlType() != ControlTypes::Container)
				Manager::instance()->mFocus = control;

			control->mouseDown(button);

			if (cc != 0)
			{
				if (button == 4)
					cc->scrollUp();
				else if (button == 3)
					cc->scrollDown();
			}
		}
		else if (state == 1)
		{
			control->mouseUp(button);

			if (cc != 0)
			{
				if (button == 4)
					cc->scrollUp();
				else if (button == 3)
					cc->scrollDown();
			}
		}
	}
}

void Manager::glutMouseMove(int x, int y)
{
	float point[2] = { x, Manager::instance()->mViewSize[1] - y };

	static Control* lastHovered = 0;
	Control* control = Manager::instance()->getTopControlAt(point);

	if (control != lastHovered && lastHovered != 0)
		lastHovered->mouseOut();
	if (control != 0)
		control->mouseIn();

	lastHovered = control;
}
#endif

}
