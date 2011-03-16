/*
 * Controls
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "Controls.h"
#include "Font.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>

using namespace ui;


/******************************************************************************************/
/*** box_t																			   ****/
/******************************************************************************************/
bool box_t::isPointInBox(float point[2])
{
	if (point[0] < this->hitbox[0]) return false;
	if (point[0] > this->hitbox[0] + this->hitbox[2]) return false;

	if (point[1] < this->hitbox[1]) return false;
	if (point[1] > this->hitbox[1] + this->hitbox[3]) return false;

	return true;
}


/******************************************************************************************/
/*** Control																		   ****/
/******************************************************************************************/
Control::Control(int type)
	: mType(type)
{
	this->setPosition(0, 0);
	this->setSize(64, 24);

	GuiManager::instance()->addControl(this);
	this->box.font = GuiManager::instance()->sDefaultFont;
}

Control::Control(int type, int x, int y, int w, int h)
	: mType(type)
{
	this->setPosition(x, y);
	this->setSize(w, h);

	GuiManager::instance()->addControl(this);
	this->box.font = GuiManager::instance()->sDefaultFont;
}

Control::~Control()
{
	GuiManager::instance()->removeControl(this);
}

void Control::renderControl()
{
	// ToDo : some extra stuff which is control wide
	this->render();
}

void Control::position(float pos[2])
{
	pos[0] = this->box.hitbox[0];
	pos[1] = this->box.hitbox[1];
}

void Control::setPosition(float pos[2])
{
	this->box.hitbox[0] = pos[0];
	this->box.hitbox[1] = pos[1];
}

void Control::setPosition(float x, float y)
{
	this->box.hitbox[0] = x;
	this->box.hitbox[1] = y;
}

void Control::size(float size[2])
{
	size[0] = this->box.hitbox[2];
	size[1] = this->box.hitbox[3];
}

void Control::setSize(float size[2])
{
	this->box.hitbox[2] = size[0];
	this->box.hitbox[3] = size[1];
}

void Control::setSize(float w, float h)
{
	this->box.hitbox[2] = (w > 10 ? w : 10);
	this->box.hitbox[3] = (h > 10 ? h : 10);
}

void Control::updateBox()
{
	this->box.boxPosition[0] = this->box.hitbox[0];
	this->box.boxPosition[1] = this->box.hitbox[1];
	this->box.boxSize[0] = this->box.hitbox[2];
	this->box.boxSize[1] = this->box.hitbox[3];
}

void Control::renderBox(bool ignoreState)
{
	this->updateBox();

	float transx = this->box.boxPosition[0];
	float transy = this->box.boxPosition[1];

	glPushMatrix();
	glTranslatef(transx, transy, 0);
	glBegin(GL_QUADS);

	if (this->box.state & BoxState::Hovered && ignoreState == false)
		glColor3f(107.0f / 255.0f, 107.0f / 255.0f, 107.0f / 255.0f);
	else
		glColor3f(82.0f / 255.0f, 82.0f / 255.0f, 82.0f / 255.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(this->box.boxSize[0], 0.0f);

	if (this->box.state & BoxState::Hovered && ignoreState == false)
		glColor3f(82.0f / 255.0f, 82.0f / 255.0f, 82.0f / 255.0f);
	else
		glColor3f(107.0f / 255.0f, 107.0f / 255.0f, 107.0f / 255.0f);
	glVertex2f(this->box.boxSize[0], this->box.boxSize[1]);
	glVertex2f(0.0f, this->box.boxSize[1]);

	glEnd();


	glBegin(GL_LINES);
	glColor3f(72.0f / 255.0f, 72.0f / 255.0f, 72.0f / 255.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(this->box.boxSize[0], 0.0f);

	glVertex2f(this->box.boxSize[0], 0.0f);
	glVertex2f(this->box.boxSize[0], this->box.boxSize[1]);

	glVertex2f(this->box.boxSize[0], this->box.boxSize[1]);
	glVertex2f(0.0f, this->box.boxSize[1]);

	glVertex2f(0.0f, this->box.boxSize[1]);
	glVertex2f(0.0f, 0.0f);


	glColor3f(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(this->box.boxSize[0]-1.0f, 1.0f);

	glVertex2f(this->box.boxSize[0]-1.0f, 1.0f);
	glVertex2f(this->box.boxSize[0]-1.0f, this->box.boxSize[1]-1.0f);

	glVertex2f(this->box.boxSize[0]-1.0f, this->box.boxSize[1]-1.0f);
	glVertex2f(1.0f,this-> box.boxSize[1]-1.0f);

	glVertex2f(1.0f, this->box.boxSize[1]-1.0f);
	glVertex2f(1.0f, 1.0f);


	glColor3f(85.0f / 255.0f, 83.0f / 255.0f, 80.0f / 255.0f);
	glVertex2f(2.0f, 2.0f);
	glVertex2f(this->box.boxSize[0]-2.0f, 2.0f);

	glVertex2f(this->box.boxSize[0]-2.0f, 2.0f);
	glVertex2f(this->box.boxSize[0]-2.0f, this->box.boxSize[1]-2.0f);

	glVertex2f(this->box.boxSize[0]-2.0f, this->box.boxSize[1]-2.0f);
	glVertex2f(2.0f, this->box.boxSize[1]-2.0f);

	glVertex2f(2.0f, this->box.boxSize[1]-2.0f);
	glVertex2f(2.0f, 2.0f);
	glEnd();

	glPopMatrix();
}

void Control::renderText(float x, float y, const char *text, unsigned int color)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->box.font->drawText(x, y, text, color);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}


/******************************************************************************************/
/*** Text Control																	   ****/
/******************************************************************************************/
Label::Label(const char* text, int type)
	: Control(type), mText(0)
{
	this->setText(text);
}

Label::Label(const char* text, int x, int y, int w, int h, int type)
	: Control(type, x, y, w, h), mText(0)
{
	this->setText(text);
	this->box.boxSize[0] = this->box.boxSize[1] = 0;
}

Label::~Label()
{
	if (this->mText != 0)
		delete [] this->mText;
}

void Label::render()
{
	int length = this->box.font->getTextLength(this->mText);
	int height = this->box.font->getTextHeight(this->mText);

	Control::renderText(this->box.hitbox[0]+this->box.hitbox[2]/2.0f - float(length)/2.0f,
			this->box.hitbox[1]+this->box.hitbox[3]/2.0f - float(height) / 4.0f,
			this->mText, RGBA(255, 255, 255, 255));
}

const char* Label::text() const
{
	return this->mText;
}

void Label::setText(const char* text)
{
	if (this->mText != 0)
		delete [] this->mText;

	int len = strlen(text) + 1;
	this->mText = new char[len];
	for (int i = 0; i < len; i++) this->mText[i] = 0;
	for (int i = 0; i < len; i++)
		this->mText[i] = text[i];
}


/******************************************************************************************/
/*** Button																			   ****/
/******************************************************************************************/
Button::Button(int x, int y, int w, int h, const char* text)
	: Label(text, x, y, w, h, ControlTypes::Button), Click(ClickEvent(this))
{
}

Button::~Button()
{
}

void Button::render()
{
	this->renderBox(false);
	int length = this->box.font->getTextLength(this->mText);
	int height = this->box.font->getTextHeight(this->mText);

	Control::renderText(this->box.hitbox[0]+this->box.hitbox[2]/2.0f - float(length)/2.0f,
			this->box.hitbox[1]+this->box.hitbox[3]/2.0f - float(height) / 4.0f,
			this->mText, RGBA(255, 255, 255, 255));
}



/******************************************************************************************/
/*** Checkbox																		   ****/
/******************************************************************************************/
Checkbox::Checkbox(int x, int y, int w, int h, const char* text)
	: Label(text, x, y, w, h, ControlTypes::Checkbox), StateChanged(StateChangedEvent(this))
{
}

Checkbox::~Checkbox()
{
}

void Checkbox::render()
{
	this->renderBox(false);

	if (this->mChecked)
	{
		glColor3f(0.3f, 0.7f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2f(this->box.boxPosition[0]+3, this->box.boxPosition[1]+3);
		glVertex2f(this->box.boxPosition[0]+this->box.boxSize[0]-4, this->box.boxPosition[1]+3);
		glVertex2f(this->box.boxPosition[0]+this->box.boxSize[0]-4, this->box.boxPosition[1]+this->box.boxSize[1]-4);
		glVertex2f(this->box.boxPosition[0]+3, this->box.boxPosition[1]+this->box.boxSize[1]-4);
		glEnd();
	}

	int height = this->box.font->getTextHeight(this->mText);

	Control::renderText(this->box.boxPosition[0]+this->box.boxSize[0], this->box.boxPosition[1]+this->box.boxSize[1]/2.0f - float(height) / 4.0f, this->mText, RGBA(255, 255, 255, 255));
}

bool Checkbox::checked()
{
	return this->mChecked;
}

void Checkbox::setChecked(bool state)
{
	this->mChecked = state;
	EventArgs e;
	this->StateChanged(&e);
}

void Checkbox::toggleChecked()
{
	if (this->mChecked)
		this->setChecked(false);
	else
		this->setChecked(true);
}

void Checkbox::updateBox()
{
	this->box.boxPosition[0] = this->box.hitbox[0];
	this->box.boxPosition[1] = this->box.hitbox[1];
	this->box.boxSize[0] = 16;
	this->box.boxSize[1] = 16;
}


/******************************************************************************************/
/*** Textbox																		   ****/
/******************************************************************************************/
Textbox::Textbox(int x, int y, int w, int h, const char* text)
	: Label(text, x, y, w, h, ControlTypes::Textbox), mTextLength(0), mCursorIndex(0), mCursorPosition(0)
{
	while (text[this->mTextLength] != 0)
		this->mTextLength++;
	this->mBufferLength = this->mTextLength;
	this->mCursorIndex = this->mTextLength;
	this->mCursorPosition = this->box.font->getTextLength(text, this->mCursorIndex);
}

Textbox::~Textbox()
{
}

void Textbox::render()
{
	this->renderBox(false);
	int height = this->box.font->getTextHeight(this->mText);

	glColor3f(0.3f, 0.7f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(this->box.boxPosition[0]+3, this->box.boxPosition[1]+3);
	glVertex2f(this->box.boxPosition[0]+this->box.boxSize[0]-4, this->box.boxPosition[1]+3);
	glVertex2f(this->box.boxPosition[0]+this->box.boxSize[0]-4, this->box.boxPosition[1]+this->box.boxSize[1]-4);
	glVertex2f(this->box.boxPosition[0]+3, this->box.boxPosition[1]+this->box.boxSize[1]-4);
	glEnd();

	Control::renderText(this->box.boxPosition[0]+4, this->box.boxPosition[1]+this->box.boxSize[1] - float(height), this->mText, RGBA(0, 0, 0, 255));

	glBegin(GL_QUADS);
	glVertex2f(this->box.boxPosition[0] + this->mCursorPosition+4, this->box.boxPosition[1]+this->box.boxSize[1]-4);
	glVertex2f(this->box.boxPosition[0] + this->mCursorPosition+4+2, this->box.boxPosition[1]+this->box.boxSize[1]-4);
	glVertex2f(this->box.boxPosition[0] + this->mCursorPosition+4+2, this->box.boxPosition[1]+this->box.boxSize[1]-20);
	glVertex2f(this->box.boxPosition[0] + this->mCursorPosition+4, this->box.boxPosition[1]+this->box.boxSize[1]-20);
	glEnd();

}

void Textbox::setText(const char* text)
{
	while (text[this->mTextLength] != 0)
		this->mTextLength++;
	this->mBufferLength = this->mTextLength;
	Label::setText(text);
}

void Textbox::addChar(char c)
{
	if (this->mTextLength >= this->mBufferLength)
	{
		this->mBufferLength *= 2;
		char* buff = new char[this->mBufferLength];
		for (int i = 0; i < this->mBufferLength; i++) buff[i] = 0;
		for (int i = 0; i < this->mTextLength; i++)
			buff[i] = this->mText[i];
		delete []this->mText;
		this->mText = buff;
	}
	for (int i = this->mBufferLength; i > this->mCursorIndex; i--) this->mText[i] = this->mText[i-1];
	this->mText[this->mCursorIndex++] = c;
	this->mTextLength++;
	this->mCursorPosition = this->box.font->getTextLength(this->mText, this->mCursorIndex);
}

char Textbox::removeChar()
{
	char res = this->mText[this->mTextLength-1];

	if (this->mCursorIndex >= 1)
	{
		for (int i = this->mCursorIndex - 1; i < this->mBufferLength; i++)
			this->mText[i] = this->mText[i+1];
		this->mTextLength--;
		this->mCursorIndex--;

		this->mCursorPosition = 0;
		if (this->mCursorIndex > 0)
			this->mCursorPosition = this->box.font->getTextLength(this->mText, this->mCursorIndex);
	}

	return res;
}

void Textbox::moveCursor(int amount)
{
	this->mCursorIndex += amount;
	if (this->mCursorIndex <= 0) this->mCursorIndex = 0;
	if (this->mCursorIndex > this->mTextLength) this->mCursorIndex = this->mTextLength;

	this->mCursorPosition = 0;
	if (this->mCursorIndex > 0)
		this->mCursorPosition = this->box.font->getTextLength(this->mText, this->mCursorIndex);
}


/******************************************************************************************/
/*** Valuebox																		   ****/
/******************************************************************************************/
Valuebox::Valuebox(int x, int y, int w, int h, float value, float min, float max)
	: Control(ControlTypes::Valuebox, x, y, w, h), mValue(value), mMinValue(min), mMaxValue(max)
{
	this->mInput[0] = 0;
}

Valuebox::~Valuebox()
{
}

void Valuebox::render()
{
	this->renderBox(true);

	char str[32] = { 0 };
	if (this->mInput[0] == 0)
		sprintf(str, "%0.4f", this->mValue);
	else
		strcpy(str, this->mInput);

	int length = this->box.font->getTextLength(str);
	int height = this->box.font->getTextHeight("W");

	Control::renderText(this->box.boxPosition[0]+this->box.boxSize[0]/2.0f - float(length)/2.0f,
			this->box.boxPosition[1]+this->box.boxSize[1]/2.0f - float(height) / 4.0f-4,
			str, RGBA(255, 255, 255, 255));
}

float Valuebox::value()
{
	return this->mValue;
}

void Valuebox::setValue(float value)
{
	this->mValue = value;
	if (this->mValue < this->mMinValue) this->mValue = this->mMinValue;
	if (this->mValue > this->mMaxValue) this->mValue = this->mMaxValue;
}

float Valuebox::minValue()
{
	return this->mMinValue;
}

void Valuebox::setMinValue(float min)
{
	this->mMinValue = min;
}

float Valuebox::maxValue()
{
	return this->mMaxValue;
}

void Valuebox::setMaxValue(float max)
{
	this->mMaxValue = max;
}

void Valuebox::addInput(char key)
{
	int i = 0;
	while (this->mInput[i] != 0 && i < 31)
		i++;
	if (key == 8)
	{
		this->mInput[i-1] = '\0';
	}
	else if ((key >= '0' && key <= '9') || key == '.')
	{
		this->mInput[i] = key;
		this->mInput[i+1] = 0;
	}
	else if (key == 13)
	{
		float val = atof(this->mInput);
		this->setValue(val);
		this->mInput[0] = 0;
	}
}
