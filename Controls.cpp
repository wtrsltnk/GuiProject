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

class Clipper
{
public:
	Clipper(float hitbox[4])
	{
		for (int i = 0; i < 4; i++)
			this->hitbox[i] = hitbox[i];
		glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
		glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
		Clipper::stack++;
		// only draw to stencil buffer
		glColorMask(0, 0, 0, 0);
		glStencilMask(0xFF);
		glBegin(GL_QUADS);
		glVertex2f(this->hitbox[0], this->hitbox[1]);
		glVertex2f(this->hitbox[0]+this->hitbox[2], this->hitbox[1]);
		glVertex2f(this->hitbox[0]+this->hitbox[2], this->hitbox[1]+this->hitbox[3]);
		glVertex2f(this->hitbox[0], this->hitbox[1]+this->hitbox[3]);
		glEnd();

		// Stencil clipper drawn,
		glColorMask(1, 1, 1, 1);
		glStencilMask(0);
		// now only draw stuff that's that has the right clipper value
		glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	virtual ~Clipper()
	{
		// Decrement anything we previously incremented
		glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
		glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
		Clipper::stack--;
		// Only draw to stencil buffer
		glColorMask(0, 0, 0, 0);
		glStencilMask(0xFF);
		glBegin(GL_QUADS);
		glVertex2f(this->hitbox[0], this->hitbox[1]);
		glVertex2f(this->hitbox[0]+this->hitbox[2], this->hitbox[1]);
		glVertex2f(this->hitbox[0]+this->hitbox[2], this->hitbox[1]+this->hitbox[3]);
		glVertex2f(this->hitbox[0], this->hitbox[1]+this->hitbox[3]);
		glEnd();

		// now draw on regular color buffer again,
		// stencil buffer should be the same as before constructor call
		glColorMask(1, 1, 1, 1);
		glStencilMask(0);
		glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	}

private:
	float hitbox[4];
	static int stack;
};

int Clipper::stack = 0;


/******************************************************************************************/
/*** GuiEventArgs																	   ****/
/******************************************************************************************/
GuiEventArgs::GuiEventArgs(Control* ctr)
	: mControl(ctr)
{
}

GuiEventArgs::~GuiEventArgs()
{
}

Control* GuiEventArgs::control()
{
	return this->mControl;
}


/******************************************************************************************/
/*** Control																		   ****/
/******************************************************************************************/
Control::Control(int type)
	: mParent(0), mType(type)
{
	this->setPosition(0, 0);
	this->setSize(64, 24);

	GuiManager::instance()->addControl(this);
	this->mBox.font = GuiManager::instance()->sDefaultFont;
}

Control::Control(int type, int x, int y, int w, int h)
	: mParent(0), mType(type)
{
	this->setPosition(x, y);
	this->setSize(w, h);

	GuiManager::instance()->addControl(this);
	this->mBox.font = GuiManager::instance()->sDefaultFont;
}

Control::~Control()
{
	GuiManager::instance()->removeControl(this);
}

void Control::renderControl()
{
	Clipper c(this->mBox.hitbox);
	this->render();

	if (this == GuiManager::instance()->mFocus)
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
	if (this->mParent == 0)
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


	glColor3f(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(this->mBox.boxSize[0]-1.0f, 1.0f);

	glVertex2f(this->mBox.boxSize[0]-1.0f, 1.0f);
	glVertex2f(this->mBox.boxSize[0]-1.0f, this->mBox.boxSize[1]-1.0f);

	glVertex2f(this->mBox.boxSize[0]-1.0f, this->mBox.boxSize[1]-1.0f);
	glVertex2f(1.0f,this-> mBox.boxSize[1]-1.0f);

	glVertex2f(1.0f, this->mBox.boxSize[1]-1.0f);
	glVertex2f(1.0f, 1.0f);


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

bool Control::isPointInBox(float point[2])
{
	float scroll = 0;
	if (this->mParent != 0 && dynamic_cast<VerticalContainer*>(this->mParent) != 0)
		scroll = this->mParent->getScroll();

	if (point[0] < this->mBox.hitbox[0]) return false;
	if (point[0] > this->mBox.hitbox[0] + this->mBox.hitbox[2]) return false;

	if (point[1] > this->mBox.hitbox[1] + scroll + this->mBox.hitbox[3]) return false;
	if (point[1] < this->mBox.hitbox[1] + scroll) return false;

	return true;
}



/******************************************************************************************/
/*** Container																		   ****/
/******************************************************************************************/
Container::Container(int x, int y, int w, int h)
	: Control(ControlTypes::Container, x, y, w, h), mScroll(0), mChildHeight(0), mPadding(4)
{
}

Container::~Container()
{
}

void Container::renderScrollbar(float& scrollbarWidth)
{
	if (this->mChildHeight > this->height())
	{
		float h = fmax(this->mChildHeight, this->height());
		float sbh = this->height() / h;

		glColor3f(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f);
		glBegin(GL_QUADS);
		glVertex2f(this->mBox.hitbox[0]+this->mBox.hitbox[2]-10, this->mBox.hitbox[1]+this->mBox.hitbox[3] - 4 - (this->mScroll*sbh));
		glVertex2f(this->mBox.hitbox[0]+this->mBox.hitbox[2]-4, this->mBox.hitbox[1]+this->mBox.hitbox[3] - 4 - (this->mScroll*sbh));
		glVertex2f(this->mBox.hitbox[0]+this->mBox.hitbox[2]-4, this->mBox.hitbox[1]+this->mBox.hitbox[3] - ((this->mBox.hitbox[3] - 8 + this->mScroll)*sbh));
		glVertex2f(this->mBox.hitbox[0]+this->mBox.hitbox[2]-10, this->mBox.hitbox[1]+this->mBox.hitbox[3] - ((this->mBox.hitbox[3] - 8 + this->mScroll)*sbh));
		glEnd();
		scrollbarWidth = 7;
	}
	else
		scrollbarWidth = 0;
}

void Container::addControl(Control* ctr)
{
	if (ctr->parent() != 0)
		ctr->parent()->removeControl(ctr);
	this->mControls.push_back(ctr);
	this->updateChildControls();
	ctr->mParent = this;
}

void Container::removeControl(Control* ctr)
{
	for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		if ((*itr) == ctr)
		{
			this->mControls.erase(itr);
			ctr->mParent = 0;
			break;
		}
	}
}

void Container::scrollUp()
{
	if (this->mChildHeight > this->height())
	{
		this->mScroll += 5.0f;
		float diff = this->mChildHeight - this->height();
		if (this->mScroll > diff)
			this->mScroll = diff;
	}
	else if (this->mParent != 0)
	{
		this->mParent->scrollUp();
	}
}

void Container::scrollDown()
{
	if (this->mChildHeight > this->height())
	{
		this->mScroll -= 5.0f;
		if (this->mScroll < 0)
			this->mScroll = 0;
	}
	else if (this->mParent != 0)
	{
		this->mParent->scrollDown();
	}
}

float Container::getScroll()
{
	if (this->mParent != 0)
		return this->mParent->getScroll() + this->mScroll;
	return this->mScroll;
}

void Container::setSize(float w, float h)
{
	Control::setSize(w, h);
	this->updateChildControls();
}

float Container::padding()
{
	return this->mPadding;
}

void Container::setPadding(float padding)
{
	this->mPadding = padding;
}



/******************************************************************************************/
/*** VerticalContainer																		   ****/
/******************************************************************************************/
VerticalContainer::VerticalContainer(int x, int y, int w, int h)
	: Container(x, y, w, h)
{
}

VerticalContainer::~VerticalContainer()
{
}

void VerticalContainer::render()
{
	float scrollbarWidth = 0;
	this->renderBox(false);

	this->renderScrollbar(scrollbarWidth);
	
	float hitbox[4] = {
			this->mBox.hitbox[0]+3,
			this->mBox.hitbox[1]+3,
			this->mBox.hitbox[2]-6 - scrollbarWidth,
			this->mBox.hitbox[3]-6
	};
	Clipper c(hitbox);

	glPushMatrix();
	glTranslatef(0, this->mScroll, 0);
	for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
		(*itr)->renderControl();
	glPopMatrix();
}

void VerticalContainer::updateChildControls()
{
	float scrollbarWidth = 0;
	float x = this->mBox.hitbox[0];
	float y = this->mBox.hitbox[1] + this->mBox.hitbox[3];

	this->mChildHeight = this->mPadding;
	for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
		this->mChildHeight += (*itr)->height() + this->mPadding;

	if (this->mChildHeight > this->height())
		scrollbarWidth = 7;

	for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		Control* c = *itr;
		c->box().hitbox[0] = x + this->mPadding;
		c->box().hitbox[1] = y - this->mPadding - c->height();
		c->box().hitbox[2] = this->width() - (this->mPadding * 2) - scrollbarWidth;
		c->updateBox();
		y -= c->height() + this->mPadding;
		VerticalContainer* cc = dynamic_cast<VerticalContainer*> (c);
		if (cc != 0)
			cc->updateChildControls();
	}
}




/******************************************************************************************/
/*** Label Control																	   ****/
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
	this->mBox.boxSize[0] = this->mBox.boxSize[1] = 0;
}

Label::~Label()
{
	if (this->mText != 0)
		delete [] this->mText;
}

void Label::render()
{
	float length = this->mBox.font->getTextLength(this->mText);
	float height = this->mBox.font->getTextHeight(this->mText);

	Control::renderText(
			this->mBox.hitbox[0] + (this->mBox.hitbox[2]/2.0f) - (length/2.0f),
			this->mBox.hitbox[1] + (this->mBox.hitbox[3]/2.0f) - (height/4.0f),
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
	float length = this->mBox.font->getTextLength(this->mText);
	float height = this->mBox.font->getTextHeight(this->mText);

	Control::renderText(
			this->mBox.hitbox[0] + (this->mBox.hitbox[2]/2.0f) - (length/2.0f),
			this->mBox.hitbox[1] + (this->mBox.hitbox[3]/2.0f) - (height/4.0f),
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
		glVertex2f(this->mBox.boxPosition[0]+3, this->mBox.boxPosition[1]+3);
		glVertex2f(this->mBox.boxPosition[0]+this->mBox.boxSize[0]-4, this->mBox.boxPosition[1]+3);
		glVertex2f(this->mBox.boxPosition[0]+this->mBox.boxSize[0]-4, this->mBox.boxPosition[1]+this->mBox.boxSize[1]-4);
		glVertex2f(this->mBox.boxPosition[0]+3, this->mBox.boxPosition[1]+this->mBox.boxSize[1]-4);
		glEnd();
	}

	Control::renderText(
			this->mBox.boxPosition[0] + (this->mBox.boxSize[0]) + 2.0f,
			this->mBox.boxPosition[1] + (this->mBox.boxSize[1]/2.0f) - 5.0f,
			this->mText, RGBA(255, 255, 255, 255));
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
	this->mBox.boxPosition[0] = this->mBox.hitbox[0];
	this->mBox.boxPosition[1] = this->mBox.hitbox[1] + (this->mBox.hitbox[3]/2.0f) - 8.0f;
	this->mBox.boxSize[0] = 16;
	this->mBox.boxSize[1] = 16;
}


/******************************************************************************************/
/*** Textbox																		   ****/
/******************************************************************************************/
Textbox::Textbox(int x, int y, int w, int h, const char* text)
	: Label(text, x, y, w, h, ControlTypes::Textbox), TextChanged(TextChangedEvent(this)), mTextLength(0), mCursorIndex(0), mCursorPosition(0), mScroll(0), mPadding(4)
{
	while (text[this->mTextLength] != 0)
		this->mTextLength++;
	this->mBufferLength = this->mTextLength;
	this->setCursorIndex(this->mTextLength);
}

Textbox::~Textbox()
{
}

void Textbox::render()
{
	this->renderBox(false);
	float height = this->mBox.font->getTextHeight(this->mText);

	float hitbox[4] = {
			this->mBox.hitbox[0]+2,
			this->mBox.hitbox[1]+2,
			this->mBox.hitbox[2]-4,
			this->mBox.hitbox[3]-4
	};
	Clipper c(hitbox);

	glColor3f(0.3f, 0.7f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(hitbox[0], hitbox[1]);
	glVertex2f(hitbox[0]+hitbox[2], hitbox[1]);
	glVertex2f(hitbox[0]+hitbox[2], hitbox[1]+hitbox[3]);
	glVertex2f(hitbox[0], hitbox[1]+hitbox[3]);
	glEnd();

	Control::renderText(
			this->mBox.boxPosition[0] + this->mPadding + this->mScroll,
			this->mBox.boxPosition[1] + this->mBox.boxSize[1] - height,
			this->mText, RGBA(0, 0, 0, 255));

	// Render cursor
	glBegin(GL_QUADS);
	glVertex2f(this->mBox.boxPosition[0] + this->mCursorPosition + 4 + this->mScroll, this->mBox.boxPosition[1]+this->mBox.boxSize[1] - 4);
	glVertex2f(this->mBox.boxPosition[0] + this->mCursorPosition + 4 + 2 + this->mScroll, this->mBox.boxPosition[1]+this->mBox.boxSize[1] - 4);
	glVertex2f(this->mBox.boxPosition[0] + this->mCursorPosition + 4 + 2 + this->mScroll, this->mBox.boxPosition[1]+this->mBox.boxSize[1] - 20);
	glVertex2f(this->mBox.boxPosition[0] + this->mCursorPosition + 4 + this->mScroll, this->mBox.boxPosition[1]+this->mBox.boxSize[1] - 20);
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
		for (int i = 0; i < this->mBufferLength; i++)
			buff[i] = 0;
		for (int i = 0; i < this->mTextLength; i++)
			buff[i] = this->mText[i];
		delete []this->mText;
		this->mText = buff;
	}
	for (int i = this->mBufferLength; i > this->mCursorIndex; i--)
		this->mText[i] = this->mText[i-1];
	this->mText[this->mCursorIndex] = c;
	this->mTextLength++;
	this->setCursorIndex(this->mCursorIndex+1);
}

char Textbox::removeChar()
{
	char res = this->mText[this->mTextLength-1];

	if (this->mCursorIndex >= 1)
	{
		for (int i = this->mCursorIndex - 1; i < this->mBufferLength; i++)
			this->mText[i] = this->mText[i+1];
		this->mTextLength--;
		this->setCursorIndex(this->mCursorIndex-1);
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
		this->mCursorPosition = this->mBox.font->getTextLength(this->mText, this->mCursorIndex);

	this->setCursorIndex(this->mCursorIndex);
}

void Textbox::setCursorIndex(int index)
{
	float textLength = this->mBox.font->getTextLength(this->mText, this->mCursorIndex);
	this->mCursorIndex = index;
	if (this->mCursorIndex <= 0)
	{
		this->mCursorIndex = 0;
		this->mCursorPosition = 0;
	}
	else
		this->mCursorPosition = textLength;

	if (textLength < this->width())
		this->mScroll = 0;

	float combine = this->mCursorPosition + this->mScroll;
	float diff = combine + 10 - this->mBox.hitbox[2];
	if (diff >= 0)
		this->mScroll -= diff;
	else if (combine < 0)
		this->mScroll -= combine;
}

void Textbox::updateBox()
{
	Control::updateBox();
	this->setCursorIndex(this->mCursorIndex);
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

	int length = this->mBox.font->getTextLength(str);
	int height = this->mBox.font->getTextHeight("W");

	Control::renderText(this->mBox.boxPosition[0]+this->mBox.boxSize[0]/2.0f - float(length)/2.0f,
			this->mBox.boxPosition[1]+this->mBox.boxSize[1]/2.0f - float(height) / 4.0f-4,
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


/******************************************************************************************/
/*** Listbox																		   ****/
/******************************************************************************************/
Listbox::Listbox(int x, int y, int w, int h)
	: Control(ControlTypes::Listbox, x, y, w, h)
{
}

Listbox::~Listbox()
{
}

void Listbox::render()
{
	this->renderBox(true);
}

int Listbox::addItem(const char* text, void* data)
{
	this->mItems.push_back(Listbox::ListboxItem(text, data));

	return this->mItems.size() - 1;
}

void Listbox::removeItem(int index)
{
	for (std::vector<ListboxItem>::iterator itr = this->mItems.begin(); itr != this->mItems.end(); ++itr)
	{
//		if ((*itr).mText == this->mItems.at(index))
//		{
//			this->mItems.erase(itr);
//			break;
//		}
	}
}
