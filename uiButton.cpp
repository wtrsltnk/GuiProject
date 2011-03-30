#include "uiControls.h"
#include "uiFont.h"

namespace ui
{

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

void Button::mouseDown(Mouse::Button button)
{
	if (button == 0)
	{
		event::EventArgs e;
		this->Click(&e);
	}
	Control::mouseDown(button);
}

}
