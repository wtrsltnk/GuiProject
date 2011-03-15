/*
 * box.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "box.h"
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
	if (point[0] > this->hitbox[2]) return false;

	if (point[1] < this->hitbox[1]) return false;
	if (point[1] > this->hitbox[3]) return false;

	return true;
}


/******************************************************************************************/
/*** Control																		   ****/
/******************************************************************************************/
Control::Control(int type)
	: mType(type)
{
	GuiManager::instance()->addControl(this);
	this->box.font = GuiManager::instance()->sDefaultFont;
}

Control::~Control()
{
	GuiManager::instance()->removeControl(this);
}

void Control::renderBox(bool ignoreState)
{
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
TextControl::TextControl(int type, const char* text)
	: Control(type), mText(0)
{
	this->setText(text);
}

TextControl::~TextControl()
{
	if (this->mText != 0)
		delete [] this->mText;
}

const char* TextControl::text() const
{
	return this->mText;
}

void TextControl::setText(const char* text)
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
	: TextControl(ControlTypes::Button, text), Click(ClickEvent(this))
{
	this->box.hitbox[0] = x;
	this->box.hitbox[1] = y;
	this->box.hitbox[2] = x + w;
	this->box.hitbox[3] = y + h;

	this->box.boxPosition[0] = x;
	this->box.boxPosition[1] = y;
	this->box.boxSize[0] = w;
	this->box.boxSize[1] = h;
}

Button::~Button()
{
}

void Button::render()
{
	this->renderBox(false);
	int length = this->box.font->getTextLength(this->mText);
	int height = this->box.font->getTextHeight(this->mText);

	Control::renderText(this->box.boxPosition[0]+this->box.boxSize[0]/2.0f - float(length)/2.0f,
			this->box.boxPosition[1]+this->box.boxSize[1]/2.0f - float(height) / 4.0f,
			this->mText, RGBA(255, 255, 255, 255));
}



/******************************************************************************************/
/*** Checkbox																		   ****/
/******************************************************************************************/
Checkbox::Checkbox(int x, int y, int w, int h, const char* text)
	: TextControl(ControlTypes::Checkbox, text), StateChanged(StateChangedEvent(this))
{
	this->box.hitbox[0] = x;
	this->box.hitbox[1] = y;
	this->box.hitbox[2] = x + w;
	this->box.hitbox[3] = y + h;

	this->box.boxPosition[0] = x;
	this->box.boxPosition[1] = y;
	this->box.boxSize[0] = 16;
	this->box.boxSize[1] = 16;
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


/******************************************************************************************/
/*** Textbox																		   ****/
/******************************************************************************************/
Textbox::Textbox(int x, int y, int w, int h, const char* text)
	: TextControl(ControlTypes::Textbox, text), mTextLength(0), mCursorIndex(0), mCursorPosition(0)
{
	while (text[this->mTextLength] != 0)
		this->mTextLength++;
	this->mBufferLength = this->mTextLength;
	this->mCursorIndex = this->mTextLength;
	this->mCursorPosition = this->box.font->getTextLength(text, this->mCursorIndex);

	this->box.hitbox[0] = x;
	this->box.hitbox[1] = y;
	this->box.hitbox[2] = x + w;
	this->box.hitbox[3] = y + h;

	this->box.boxPosition[0] = x;
	this->box.boxPosition[1] = y;
	this->box.boxSize[0] = w;
	this->box.boxSize[1] = h;
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
	TextControl::setText(text);
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
	: Control(ControlTypes::Valuebox), mValue(value), mMinValue(min), mMaxValue(max)
{
	this->mInput[0] = 0;

	this->box.hitbox[0] = x;
	this->box.hitbox[1] = y;
	this->box.hitbox[2] = x + w;
	this->box.hitbox[3] = y + h;

	this->box.boxPosition[0] = x;
	this->box.boxPosition[1] = y;
	this->box.boxSize[0] = w;
	this->box.boxSize[1] = h;
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
