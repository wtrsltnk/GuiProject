#include "uiControls.h"
#include "uiClipper.h"
#include <GL/gl.h>

namespace ui
{

FreeContainer::FreeContainer(int x, int y, int w, int h)
	: Container(ControlTypes::FreeContainer, x, y, w, h)
{
}

FreeContainer::~FreeContainer()
{
}

void FreeContainer::render()
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
	glTranslatef(0,  this->scrollbar.scroll(), 0);
	for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
		(*itr)->renderControl();
	glPopMatrix();
}

void FreeContainer::updateChildControls()
{
}

}
