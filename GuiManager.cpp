/*
 * GuiManager.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "GuiManager.h"
#include "MainWindow.h"
#include "Font.h"
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

	for (std::vector<Control*>::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		(*itr)->renderControl();
	}

	if (this->mFocus != 0)
	{
		float transx = this->mFocus->box.boxPosition[0];
		float transy = this->mFocus->box.boxPosition[1];

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
		glColor4f(0.0f, 0.6f, 1.0f, 0.1f);
		glVertex2f(transx+1, transy+1);
		glVertex2f(transx + this->mFocus->box.boxSize[0]-2, transy+1);
		glVertex2f(transx + this->mFocus->box.boxSize[0]-2, transy + this->mFocus->box.boxSize[1]-2);
		glVertex2f(transx+1, transy + this->mFocus->box.boxSize[1]-2);
		glEnd();
		glDisable(GL_BLEND);
	}

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
				tb->removeChar();
			else if (key >= 32 && key < 128)
				tb->addChar(key);
		}
		else if (GuiManager::sInstance->mFocus->getType() == ControlTypes::Valuebox)
		{
			Valuebox* vb = (Valuebox*)GuiManager::sInstance->mFocus;
			vb->addInput(key);
		}
	}
	glutPostRedisplay();
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
	glutPostRedisplay();
}

void GuiManager::glutMouseClick(int button, int state, int x, int y)
{
	float point[2] = { x, GuiManager::sInstance->mViewSize[1] - y };

	for (std::vector<Control*>::iterator itr = GuiManager::sInstance->mControls.begin(); itr != GuiManager::sInstance->mControls.end(); ++itr)
	{
		if ((*itr)->box.isPointInBox(point))
		{
			if (state == 0)
			{
				GuiManager::sInstance->mFocus = (*itr);
				(*itr)->box.state = BoxState::Pressed;
				if ((*itr)->getType() == ControlTypes::Checkbox)
				{
					Checkbox* c = (Checkbox*)(*itr);
					c->toggleChecked();
				}
				else if ((*itr)->getType() == ControlTypes::Button)
				{
					Button* b = (Button*)(*itr);
					EventArgs e;
					b->Click(&e);
				}
			}
			else
			{
				(*itr)->box.state = BoxState::Hovered;
			}
		}
	}
	glutPostRedisplay();
}

void GuiManager::glutMouseMove(int x, int y)
{
	float point[2] = { x, GuiManager::sInstance->mViewSize[1] - y };

	for (std::vector<Control*>::iterator itr = GuiManager::sInstance->mControls.begin(); itr != GuiManager::sInstance->mControls.end(); ++itr)
	{
		if ((*itr)->box.isPointInBox(point))
			(*itr)->box.state = BoxState::Hovered;
		else
			(*itr)->box.state = BoxState::None;
	}
	glutPostRedisplay();
}

#endif
