/*
 * uiManager.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "uiManager.h"
#include "uiFont.h"
#include "uiControls.h"
#include "../GlutApplication.h"
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

	Manager::instance()->mRoot = new FreeContainer(0, 0, 0, 0);
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

Container* Manager::getRoot()
{
	return this->mRoot;
}

Control* Manager::getTopControlAt(float point[2], Container* container)
{
	Control* result = 0;

	if (container == 0)
		container = Manager::instance()->mRoot;

	if (container != 0)
	{
		for (std::vector<Control*>::iterator itr = container->controls().begin(); itr != container->controls().end(); ++itr)
		{
			Control* c = (*itr);
			if (c->visible() && c->isPointInBox(point))
			{
				result = c;

				Container* cc = dynamic_cast<Container*>(c);
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
		this->mRoot->setSize(w, h);
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
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glLineWidth(1);
	glPointSize(1);

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

void Manager::onKeyDown(Key::Code key)
{
	if (Manager::instance() != 0 && Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyDown(key);
		KeyboardEventArgs e;
		e.key = key;
		Manager::instance()->mFocus->onKeyboardDown(&e);
	}
}

void Manager::onKeyUp(Key::Code key)
{
	if (Manager::instance() != 0 && Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyUp(key);
		KeyboardEventArgs e;
		e.key = key;
		Manager::instance()->mFocus->onKeyboardUp(&e);
	}
}

void Manager::onMouseButtonDown(Mouse::Button button)
{
	if (Manager::instance() != 0)
	{
		float point[2] = { MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY() };

		Control* control = Manager::instance()->getTopControlAt(point);

		if (control != 0)
		{
			// Only non-containers can get focus
			if (dynamic_cast<Container*>(control) == 0 &&  button == Mouse::Left)
				Manager::instance()->mFocus = control;

			control->mouseDown(button);
		}

		if (Manager::instance()->mFocus != 0)
		{
			MouseButtonEventArgs e;
			e.button = button;
			Manager::instance()->mFocus->onMouseButtonDown(&e);
		}
	}
}

void Manager::onMouseButtonUp(Mouse::Button button)
{
	if (Manager::instance() != 0)
	{
		float point[2] = { MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY() };

		Control* control = Manager::instance()->getTopControlAt(point);

		if (control != 0)
		{
			control->mouseUp(button);
		}

		if (Manager::instance()->mFocus != 0)
		{
			MouseButtonEventArgs e;
			e.button = button;
			Manager::instance()->mFocus->onMouseButtonUp(&e);
		}
	}
}

void Manager::onMouseMove(int x, int y)
{
	if (Manager::instance() != 0)
	{
		float point[2] = { MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY() };

		static Control* lastHovered = 0;
		Control* control = Manager::instance()->getTopControlAt(point);

		if (control != lastHovered && lastHovered != 0)
			lastHovered->mouseOut();
		if (control != 0)
			control->mouseIn();

		lastHovered = control;
		if (Manager::instance()->mFocus != 0)
		{
			MouseButtonEventArgs e;
			Manager::instance()->mFocus->onMouseMove(&e);
		}
	}
}

}
