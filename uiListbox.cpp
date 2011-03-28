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

	float y = 0;
	glPushMatrix();
	glTranslatef(0, this->scrollbar.scroll(), 0);
	for (std::vector<ListboxItem>::iterator itr = this->mItems.begin(); itr != this->mItems.end(); ++itr)
	{
		Control::renderText(
				this->mBox.hitbox[0] + this->mPadding,
				this->mBox.hitbox[1] + this->mBox.hitbox[3] + this->scrollbar.scroll() - 20 - y,
				(*itr).mText, RGBA(255, 255, 255, 255));
		y += 20;
	}
	glPopMatrix();
}

void Listbox::mouseDown(int button)
{
	if (button == 4)
		this->scrollbar.scrollUp();
	else if (button == 3)
		this->scrollbar.scrollDown();
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
