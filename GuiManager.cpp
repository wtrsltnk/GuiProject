/*
 * GuiManager.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "GuiManager.h"
#include "MainWindow.h"
#include "Font.h"
#include "Controls.h"
#include <GL/freeglut.h>

template <class T>
class EventManager<T>::PrivateHandler
{
public:
	PrivateHandler(EventHandler* handler, eventFn eventFn, ui::Control* control, int type) : mHandler(handler), mEventFn(eventFn), mControl(control), mEventType(type) { }

	EventHandler* mHandler;
	eventFn mEventFn;
	ui::Control* mControl;
	int mEventType;
};

template <class T>
EventManager<T>* EventManager<T>::sInstance = 0;

using namespace ui;

GuiManager::GuiManager()
	: mFocus(0), mRoot(0)
{
}

Font* GuiManager::sDefaultFont = 0;

GuiManager* GuiManager::createInstance(const char* fontpath)
{
	if (EventManager<ui::Control>::sInstance != 0)
		delete EventManager<ui::Control>::sInstance;

	EventManager<ui::Control>::sInstance = new GuiManager();
	EventManager<ui::Control>::sInstance->initialize(fontpath);

	return (GuiManager*)EventManager<ui::Control>::sInstance;
}

GuiManager* GuiManager::instance()
{
	return (GuiManager*)EventManager<ui::Control>::sInstance;
}

void GuiManager::destroyInstance()
{
	if (EventManager<ui::Control>::sInstance != 0)
		delete EventManager<ui::Control>::sInstance;
	EventManager<ui::Control>::sInstance = 0;
}

GuiManager::~GuiManager()
{
	while (this->mHandlers.empty() == false)
	{
		PrivateHandler* h = this->mHandlers.back();
		this->mHandlers.pop_back();
		delete h;
	}

	while (this->mControls.empty() == false)
	{
		// Only delete the, since deleting automatically removes them from the GuiManager
		Control* c = this->mControls.back();
		delete c;
	}

	if (this->mRoot != 0)
		delete this->mRoot;
}

void GuiManager::initialize(const char* fontpath)
{
	if (GuiManager::sDefaultFont != 0)
		delete GuiManager::sDefaultFont;

	GuiManager::sDefaultFont = new Font();
	GuiManager::sDefaultFont->initializeFont(fontpath);

	GuiManager::instance()->mRoot = new ui::VerticalContainer(20, 20, 10, 10);

#ifndef SKIP_GLUT
	glutKeyboardFunc(&GuiManager::glutKeyboardDown);
	glutKeyboardUpFunc(&GuiManager::glutKeyboardUp);
	glutSpecialFunc(&GuiManager::glutSpecialKeyboardDown);
	glutSpecialUpFunc(&GuiManager::glutSpecialKeyboardUp);
	glutMouseFunc(&GuiManager::glutMouseClick);
	glutMotionFunc(&GuiManager::glutMouseMove);
	glutPassiveMotionFunc(&GuiManager::glutMouseMove);
#endif
}

void GuiManager::addEventHandler(EventHandler* handler, eventFn method, Control* box, int eventType)
{
	this->mHandlers.push_back(new PrivateHandler(handler, method, box, eventType));
}

void GuiManager::removeEventHandler(EventHandler* handler, eventFn method, Control* box)
{
	for (std::vector<PrivateHandler*>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
	{
		PrivateHandler* h = *itr;
		if (h->mControl == box && h->mEventFn == method && h->mHandler == handler)
		{
			this->mHandlers.erase(itr);
			delete h;
			break;
		}
	}
}

void GuiManager::initiateEvent(Control* box, int eventType, EventArgs* e)
{
	for (std::vector<PrivateHandler*>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
	{
		if ((*itr)->mEventType == eventType)
		{
			if ((*itr)->mControl == box)
			{
				((*(*itr)->mHandler).*(*itr)->mEventFn)(box, e);
			}
		}
	}
}

void GuiManager::addControl(Control* ctr)
{
	ctr->updateBox();
	this->mControls.push_back(ctr);
	if (ctr->parent() == 0 && this->mRoot != 0)
		this->mRoot->addControl(ctr);
}

void GuiManager::removeControl(Control* ctr)
{
	for (std::vector<Control*>::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		if ((*itr) == ctr)
		{
			this->mControls.erase(itr);
			break;
		}
	}
	for (std::vector<PrivateHandler*>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
	{
		if ((*itr)->mControl == ctr)
		{
			this->mHandlers.erase(itr);
		}
	}
}

VerticalContainer* GuiManager::getRoot()
{
	return this->mRoot;
}

Control* GuiManager::getTopControlAt(float point[2], VerticalContainer* container)
{
	Control* result = 0;

	if (container == 0)
		container = GuiManager::instance()->mRoot;

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

void GuiManager::setupSize(int w, int h)
{
	this->mViewSize[0] = w;
	this->mViewSize[1] = h;
	if (this->mRoot != 0)
		this->mRoot->setSize(w-40, h-40);
}

void GuiManager::render()
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
void GuiManager::glutKeyboardDown(unsigned char key, int x, int y)
{
	if (GuiManager::instance()->mFocus != 0)
	{
		GuiManager::instance()->mFocus->charDown(key);
	}
}
void GuiManager::glutKeyboardUp(unsigned char key, int x, int y)
{
	if (GuiManager::instance()->mFocus != 0)
	{
		GuiManager::instance()->mFocus->charUp(key);
	}
}

void GuiManager::glutSpecialKeyboardDown(int key, int x, int y)
{
	if (GuiManager::instance()->mFocus != 0)
	{
		GuiManager::instance()->mFocus->keyDown(key);
	}
}

void GuiManager::glutSpecialKeyboardUp(int key, int x, int y)
{
	if (GuiManager::instance()->mFocus != 0)
	{
		GuiManager::instance()->mFocus->keyUp(key);
	}
}

void GuiManager::glutMouseClick(int button, int state, int x, int y)
{
	float point[2] = { x, GuiManager::instance()->mViewSize[1] - y };

	Control* control = GuiManager::instance()->getTopControlAt(point);

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
				GuiManager::instance()->mFocus = control;

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

void GuiManager::glutMouseMove(int x, int y)
{
	float point[2] = { x, GuiManager::instance()->mViewSize[1] - y };

	static Control* lastHovered = 0;
	Control* control = GuiManager::instance()->getTopControlAt(point);

	if (control != lastHovered && lastHovered != 0)
		lastHovered->mouseOut();
	if (control != 0)
		control->mouseIn();

	lastHovered = control;
}
#endif
