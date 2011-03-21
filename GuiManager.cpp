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

namespace ui
{
class EventHandler
{
public:
	EventHandler(GuiEventHandler* handler, eventFn eventFn, Control* control, int type) : mHandler(handler), mEventFn(eventFn), mControl(control), mEventType(type) { }

	GuiEventHandler* mHandler;
	eventFn mEventFn;
	Control* mControl;
	int mEventType;
};
}

using namespace ui;

GuiManager::GuiManager()
	: mFocus(0)
{
}

GuiManager* GuiManager::sInstance = 0;
Font* GuiManager::sDefaultFont = 0;

GuiManager* GuiManager::createInstance(const char* fontpath)
{
	if (GuiManager::sInstance != 0)
		delete GuiManager::sInstance;

	GuiManager::sInstance = new GuiManager();

	if (GuiManager::sDefaultFont != 0)
		delete GuiManager::sDefaultFont;
	GuiManager::sDefaultFont = new Font();
	GuiManager::sDefaultFont->initializeFont(fontpath);

#ifdef USE_GLUT
	glutKeyboardFunc(&GuiManager::glutKeyboard);
	glutSpecialFunc(&GuiManager::glutSpecialKeyboard);
	glutMouseFunc(&GuiManager::glutMouseClick);
	glutMotionFunc(&GuiManager::glutMouseMove);
	glutPassiveMotionFunc(&GuiManager::glutMouseMove);
#endif

	return GuiManager::sInstance;
}

GuiManager* GuiManager::instance()
{
	return GuiManager::sInstance;
}

void GuiManager::destroyInstance()
{
	if (GuiManager::sInstance != 0)
		delete GuiManager::sInstance;
	GuiManager::sInstance = 0;
}

GuiManager::~GuiManager()
{
	while (this->mHandlers.empty() == false)
	{
		EventHandler* h = this->mHandlers.back();
		this->mHandlers.pop_back();
		delete h;
	}

	while (this->mControls.empty() == false)
	{
		// Only delete the, since deleting automatically removes them from the GuiManager
		Control* c = this->mControls.back();
		delete c;
	}
}

void GuiManager::addEventHandler(GuiEventHandler* handler, eventFn method, Control* box, int eventType)
{
	this->mHandlers.push_back(new EventHandler(handler, method, box, eventType));
}

void GuiManager::removeEventHandler(GuiEventHandler* handler, eventFn method, Control* box)
{
	for (std::vector<EventHandler*>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
	{
		EventHandler* h = *itr;
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
	for (std::vector<EventHandler*>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
	{
		if ((*itr)->mEventType == eventType)
		{
			if ((*itr)->mControl == box)
			{
				((*(*itr)->mHandler).*(*itr)->mEventFn)((*itr)->mControl, e);
			}
		}
	}
}

void GuiManager::addControl(Control* ctr)
{
	ctr->updateBox();
	this->mControls.push_back(ctr);
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
	for (std::vector<EventHandler*>::iterator itr = this->mHandlers.begin(); itr != this->mHandlers.end(); ++itr)
	{
		if ((*itr)->mControl == ctr)
		{
			this->mHandlers.erase(itr);
		}
	}
}

Control* GuiManager::getTopControlAt(float point[2], Container* container)
{
	Control* result = 0;

	std::vector<Control*>* controls = &GuiManager::sInstance->mControls;

	if (container != 0)
	{
		controls = &container->getControls();
		for (std::vector<Control*>::iterator itr = controls->begin(); itr != controls->end(); ++itr)
		{
			Control* c = (*itr);
			if (c->isPointInBox(point))
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
	else
	{
		for (std::vector<Control*>::iterator itr = controls->begin(); itr != controls->end(); ++itr)
		{
			Control* c = (*itr);
			if (c->mParent == 0 && c->isPointInBox(point))
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

void GuiManager::setupSize(int w, int h)
{
	this->mViewSize[0] = w;
	this->mViewSize[1] = h;
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

	for (std::vector<Control*>::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		if ((*itr)->mParent == 0)
			(*itr)->renderControl();
	}

	glDisable(GL_STENCIL_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

#ifdef USE_GLUT
void GuiManager::glutKeyboard(unsigned char key, int x, int y)
{
	if (GuiManager::sInstance->mFocus != 0)
	{
		if (GuiManager::sInstance->mFocus->getType() == ControlTypes::Textbox)
		{
			Textbox* tb = (Textbox*)GuiManager::sInstance->mFocus;
			if (key == 8)
			{
				tb->removeChar();
				EventArgs e;
				tb->TextChanged(&e);
			}
			else if (key >= 32 && key < 128)
			{
				tb->addChar(key);
				EventArgs e;
				tb->TextChanged(&e);
			}
		}
		else if (GuiManager::sInstance->mFocus->getType() == ControlTypes::Valuebox)
		{
			Valuebox* vb = (Valuebox*)GuiManager::sInstance->mFocus;
			vb->addInput(key);
		}
	}
}

void GuiManager::glutSpecialKeyboard(int key, int x, int y)
{
	if (GuiManager::sInstance->mFocus != 0)
	{
		if (GuiManager::sInstance->mFocus->getType() == ControlTypes::Textbox)
		{
			Textbox* tb = (Textbox*)GuiManager::sInstance->mFocus;
			if (key == GLUT_KEY_LEFT)
				tb->moveCursor(-1);
			else if (key == GLUT_KEY_RIGHT)
				tb->moveCursor(1);
		}
		else if (GuiManager::sInstance->mFocus->getType() == ControlTypes::Valuebox)
		{
			Valuebox* vb = (Valuebox*)GuiManager::sInstance->mFocus;
			float diff = (vb->maxValue() - vb->minValue()) / 10.0f;
			if (key == GLUT_KEY_LEFT || key == GLUT_KEY_DOWN)
				vb->setValue(vb->value() - diff);
			else if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_UP)
				vb->setValue(vb->value() + diff);
		}
	}
}

void GuiManager::glutMouseClick(int button, int state, int x, int y)
{
	float point[2] = { x, GuiManager::sInstance->mViewSize[1] - y };

	Control* control = GuiManager::sInstance->getTopControlAt(point);

	if (control != 0)
	{
		if (button == 0 && state == 0)
		{
			GuiManager::sInstance->mFocus = control;
			control->mBox.state = BoxState::Pressed;
			if (control->getType() == ControlTypes::Checkbox)
			{
				Checkbox* c = (Checkbox*)control;
				c->toggleChecked();
			}
			else if (control->getType() == ControlTypes::Button)
			{
				Button* b = (Button*)control;
				EventArgs e;
				b->Click(&e);
			}
		}
		else
		{
			Container* cc = 0;
			if (control->getType() == ControlTypes::Container)
				cc = (Container*)control;
			else
				cc = control->mParent;

			if (cc != 0)
			{
				if (button == 4)
					cc->scrollUp();
				else if (button == 3)
					cc->scrollDown();
			}
			else
				control->mBox.state = BoxState::Hovered;
		}
	}
}

void GuiManager::glutMouseMove(int x, int y)
{
	float point[2] = { x, GuiManager::sInstance->mViewSize[1] - y };

	static Control* lastHovered = 0;
	Control* control = GuiManager::sInstance->getTopControlAt(point);

	if (lastHovered != 0)
		lastHovered->mBox.state = BoxState::None;

	if (control != 0 && control->getType() != ControlTypes::Container)
		control->mBox.state = BoxState::Hovered;

	lastHovered = control;
}
#endif
