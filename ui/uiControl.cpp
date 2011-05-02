/*
 * uiControls
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "uiControls.h"
#include "uiFont.h"
#include "uiClipper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>

namespace ui
{

KeyboardEventArgs::KeyboardEventArgs()
	: state(KeyboardState::currentState())
{
}

KeyboardEventArgs::~KeyboardEventArgs()
{
}

MouseButtonEventArgs::MouseButtonEventArgs()
	: state(MouseState::currentState())
{
}

MouseButtonEventArgs::~MouseButtonEventArgs()
{
}


Control::Control(int type)
	: mParent(0), mVisible(true), mType(type), onKeyboardDown(this), onKeyboardUp(this), onMouseButtonDown(this), onMouseButtonUp(this), onMouseMove(this)
{
	this->setPosition(0, 0);
	this->setSize(64, 24);

	Manager::instance()->addControl(this);
	this->mBox.font = Manager::instance()->sDefaultFont;
}

Control::Control(int type, int x, int y, int w, int h)
	: mParent(0), mVisible(true), mType(type), onKeyboardDown(this), onKeyboardUp(this), onMouseButtonDown(this), onMouseButtonUp(this), onMouseMove(this)
{
	this->setPosition(x, y);
	this->setSize(w, h);

	Manager::instance()->addControl(this);
	this->mBox.font = Manager::instance()->sDefaultFont;
}

Control::~Control()
{
	Manager::instance()->removeControl(this);
}

void Control::renderControl()
{
	if (this->mVisible)
	{
		Clipper c(this->mBox.hitbox);
		this->render();

		if (this == Manager::instance()->mFocus)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBegin(GL_QUADS);
			glColor4f(0.0f, 0.6f, 1.0f, 0.1f);
			glBegin(GL_QUADS);
			glVertex2f(this->mBox.boxPosition[0], this->mBox.boxPosition[1]);
			glVertex2f(this->mBox.boxPosition[0]+this->mBox.boxSize[0], this->mBox.boxPosition[1]);
			glVertex2f(this->mBox.boxPosition[0]+this->mBox.boxSize[0], this->mBox.boxPosition[1]+this->mBox.boxSize[1]);
			glVertex2f(this->mBox.boxPosition[0], this->mBox.boxPosition[1]+this->mBox.boxSize[1]);
			glEnd();
			glDisable(GL_BLEND);
		}
	}
}

void Control::mouseIn()
{
	this->box().state = BoxState::Hovered;
}

void Control::mouseDown(Mouse::Button button)
{
	if (button == 0)
		this->box().state = BoxState::Pressed;

	if (this->parent() != 0)
		this->parent()->mouseDown(button);
}

void Control::mouseUp(Mouse::Button button)
{
	if (button == 0)
		this->box().state = BoxState::Hovered;

	if (this->parent() != 0)
		this->parent()->mouseUp(button);
}

void Control::mouseOut()
{
	this->box().state = BoxState::Nothing;
}

void Control::keyDown(Key::Code key)
{
}

void Control::keyUp(Key::Code key)
{
}

float Control::x()
{
	return this->mBox.hitbox[0];
}

float Control::y()
{
	return this->mBox.hitbox[1];
}

void Control::setPosition(float pos[2])
{
	if (this->mParent == 0)
	{
		this->mBox.hitbox[0] = pos[0];
		this->mBox.hitbox[1] = pos[1];
	}
}

void Control::setPosition(float x, float y)
{
	FreeContainer* f = dynamic_cast<FreeContainer*> (this->mParent);
	if (this->mParent == 0 || f != 0)
	{
		this->mBox.hitbox[0] = x;
		this->mBox.hitbox[1] = y;
	}
}

float Control::width()
{
	return this->mBox.hitbox[2];
}

float Control::height()
{
	return this->mBox.hitbox[3];
}

float Control::clientHeight()
{
	return this->mBox.hitbox[3];
}

void Control::setSize(float size[2])
{
	this->setSize(size[0], size[1]);
}

void Control::setSize(float w, float h)
{
	this->mBox.hitbox[2] = (w > 10 ? w : 10);
	this->mBox.hitbox[3] = (h > 10 ? h : 10);
	if (this->mParent != 0)
	{
		if (w > this->mParent->width() - 4)
		{
			this->mBox.hitbox[2] = this->mParent->width() - 4;
		}
		this->mParent->updateChildControls();
	}
	this->updateBox();
}

bool Control::visible() const
{
	return this->mVisible;
}

void Control::setVisible(bool v)
{
	this->mVisible = v;
}

void Control::updateBox()
{
	this->mBox.boxPosition[0] = this->mBox.hitbox[0];
	this->mBox.boxPosition[1] = this->mBox.hitbox[1];
	this->mBox.boxSize[0] = this->mBox.hitbox[2];
	this->mBox.boxSize[1] = this->mBox.hitbox[3];
}

void Control::renderBox(bool ignoreState)
{
	this->updateBox();

	float transx = this->mBox.boxPosition[0];
	float transy = this->mBox.boxPosition[1];

	glPushMatrix();
	glTranslatef(transx, transy, 0);
	glBegin(GL_QUADS);

	if (this->mBox.state & BoxState::Hovered && ignoreState == false)
		glColor3f(107.0f / 255.0f, 107.0f / 255.0f, 107.0f / 255.0f);
	else
		glColor3f(82.0f / 255.0f, 82.0f / 255.0f, 82.0f / 255.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(this->mBox.boxSize[0], 0.0f);

	if (this->mBox.state & BoxState::Hovered && ignoreState == false)
		glColor3f(82.0f / 255.0f, 82.0f / 255.0f, 82.0f / 255.0f);
	else
		glColor3f(107.0f / 255.0f, 107.0f / 255.0f, 107.0f / 255.0f);
	glVertex2f(this->mBox.boxSize[0], this->mBox.boxSize[1]);
	glVertex2f(0.0f, this->mBox.boxSize[1]);

	glEnd();


	glBegin(GL_LINES);
	glColor3f(72.0f / 255.0f, 72.0f / 255.0f, 72.0f / 255.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(this->mBox.boxSize[0], 0.0f);

	glVertex2f(this->mBox.boxSize[0], 0.0f);
	glVertex2f(this->mBox.boxSize[0], this->mBox.boxSize[1]);

	glVertex2f(this->mBox.boxSize[0], this->mBox.boxSize[1]);
	glVertex2f(0.0f, this->mBox.boxSize[1]);

	glVertex2f(0.0f, this->mBox.boxSize[1]);
	glVertex2f(0.0f, 0.0f);


//	glColor3f(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f);
//	glVertex2f(1.0f, 1.0f);
//	glVertex2f(this->mBox.boxSize[0]-1.0f, 1.0f);
//
//	glVertex2f(this->mBox.boxSize[0]-1.0f, 1.0f);
//	glVertex2f(this->mBox.boxSize[0]-1.0f, this->mBox.boxSize[1]-1.0f);
//
//	glVertex2f(this->mBox.boxSize[0]-1.0f, this->mBox.boxSize[1]-1.0f);
//	glVertex2f(1.0f,this-> mBox.boxSize[1]-1.0f);
//
//	glVertex2f(1.0f, this->mBox.boxSize[1]-1.0f);
//	glVertex2f(1.0f, 1.0f);


	glColor3f(85.0f / 255.0f, 83.0f / 255.0f, 80.0f / 255.0f);
	glVertex2f(2.0f, 2.0f);
	glVertex2f(this->mBox.boxSize[0]-2.0f, 2.0f);

	glVertex2f(this->mBox.boxSize[0]-2.0f, 2.0f);
	glVertex2f(this->mBox.boxSize[0]-2.0f, this->mBox.boxSize[1]-2.0f);

	glVertex2f(this->mBox.boxSize[0]-2.0f, this->mBox.boxSize[1]-2.0f);
	glVertex2f(2.0f, this->mBox.boxSize[1]-2.0f);

	glVertex2f(2.0f, this->mBox.boxSize[1]-2.0f);
	glVertex2f(2.0f, 2.0f);
	glEnd();

	glPopMatrix();
}

void Control::renderText(float x, float y, const char *text, unsigned int color)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->mBox.font->drawText(x, y, text, color);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

bool Control::isPointInBox(float point[2], float scroll)
{
	if (this->mParent != 0 && dynamic_cast<VerticalContainer*>(this->mParent) != 0)
		scroll += this->mParent->scrollbar.globalScroll();

	if (point[0] < this->mBox.hitbox[0]) return false;
	if (point[0] > this->mBox.hitbox[0] + this->mBox.hitbox[2]) return false;

	if (point[1] > this->mBox.hitbox[1] + scroll + this->mBox.hitbox[3]) return false;
	if (point[1] < this->mBox.hitbox[1] + scroll) return false;

	return true;
}

}
