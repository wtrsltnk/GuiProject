#include "uiControls.h"
#include "uiFont.h"
#include "uiClipper.h"

namespace ui
{

Textbox::Textbox(int x, int y, int w, int h, const char* text)
	: Label(text, x, y, w, h, ControlTypes::Textbox), TextChanged(TextChangedEvent(this)), mTextLength(0), mCursorIndex(0), mCursorPosition(0), mScroll(0), mPadding(4)
{
	this->setText(text);
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

void Textbox::keyDown(int key)
{
	if (key == 0x0064)//GLUT_KEY_LEFT)
		this->moveCursor(-1);
	else if (key == 0x0066)//GLUT_KEY_RIGHT)
		this->moveCursor(1);
}

void Textbox::charDown(char c)
{
	if (c == 8)
	{
		this->removeChar();
		event::EventArgs e;
		this->TextChanged(&e);
	}
	else if (c >= 32 && c < 128)
	{
		this->addChar(c);
		event::EventArgs e;
		this->TextChanged(&e);
	}
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

}
