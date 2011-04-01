#include "uiControls.h"
#include "uiFont.h"
#include "uiClipper.h"

namespace ui
{

VerticalContainer::VerticalContainer(int x, int y, int w, int h)
	: Container(ControlTypes::VerticalContainer, x, y, w, h)
{
}

VerticalContainer::~VerticalContainer()
{
}

void VerticalContainer::render()
{
	this->renderBox(false);

	this->scrollbar.renderScrollbar();

	float hitbox[4] = {
			this->mBox.hitbox[0]+3,
			this->mBox.hitbox[1]+3,
			this->mBox.hitbox[2]-6 - this->scrollbar.width(),
			this->mBox.hitbox[3]-6
	};
	Clipper c(hitbox);

	glPushMatrix();
	glTranslatef(0, this->scrollbar.scroll(), 0);
	for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
		(*itr)->renderControl();
	glPopMatrix();
}

void VerticalContainer::updateChildControls()
{
	float x = this->mBox.hitbox[0];
	float y = this->mBox.hitbox[1] + this->mBox.hitbox[3];

	for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		Control* c = *itr;
		c->box().hitbox[0] = x + this->mPadding;
		c->box().hitbox[1] = y - this->mPadding - c->height();
		c->box().hitbox[2] = this->width() - (this->mPadding * 2) - this->scrollbar.width();
		c->updateBox();
		y -= c->height() + this->mPadding;
		Container* cc = dynamic_cast<Container*> (c);
		if (cc != 0)
			cc->updateChildControls();
	}
}

}
