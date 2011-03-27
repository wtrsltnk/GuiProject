#include "uiControls.h"
#include "uiFont.h"
#include "uiClipper.h"

namespace ui
{

Listbox::Listbox(int x, int y, int w, int h)
	: Control(ControlTypes::Listbox, x, y, w, h), scrollbar(Scrollbar(this)), mPadding(4)
{
}

Listbox::~Listbox()
{
}

void Listbox::render()
{
	this->renderBox(true);

	this->scrollbar.renderScrollbar();

	float hitbox[4] = {
			this->mBox.hitbox[0]+3,
			this->mBox.hitbox[1]+3,
			this->mBox.hitbox[2]-6 - this->scrollbar.width(),
			this->mBox.hitbox[3]-6
	};
	Clipper c(hitbox);

	glPushMatrix();
	//glTranslatef(0, this->scrollbar.scroll(), 0);
	float y = 0;
	for (std::vector<ListboxItem>::iterator itr = this->mItems.begin(); itr != this->mItems.end(); ++itr)
	{
		float length = this->mBox.font->getTextLength((*itr).mText);
		float height = this->mBox.font->getTextHeight((*itr).mText);

		Control::renderText(
				this->mBox.hitbox[0] + this->mPadding,
				this->mBox.hitbox[1] + this->mBox.hitbox[3] - height - y,
				(*itr).mText, RGBA(255, 255, 255, 255));
		y+=height;
	}
	glPopMatrix();
}

float Listbox::clientHeight()
{
	float clientHeight = this->mPadding;
	for (std::vector<ListboxItem>::iterator itr = this->mItems.begin(); itr != this->mItems.end(); ++itr)
	{
		clientHeight += this->mBox.font->getTextHeight((*itr).mText);
		clientHeight += this->mPadding;
	}
	return clientHeight;
}

void Listbox::addItem(const char* text, void* data)
{
	this->mItems.push_back(ListboxItem(text, data));
}

Listbox::ListboxItem::ListboxItem(const char* text, void* data)
	: mText(text), mData(data)
{
}

}
