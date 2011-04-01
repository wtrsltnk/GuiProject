#include "uiControls.h"
#include "uiFont.h"

namespace ui
{

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

}
