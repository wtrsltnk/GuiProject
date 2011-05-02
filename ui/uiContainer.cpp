#include "uiControls.h"
#include "uiFont.h"
#include "uiClipper.h"

namespace ui
{
	
Container::Container(int type, int x, int y, int w, int h)
	: Control(type, x, y, w, h), scrollbar(Scrollbar(this)), mPadding(4)
{
}

Container::~Container()
{
}

void Container::mouseDown(Mouse::Button button)
{
	if (button == 4)
		this->scrollbar.scrollUp();
	else if (button == 3)
		this->scrollbar.scrollDown();
}

float Container::clientHeight()
{
	float childHeight = this->padding();
	for (ControlList::iterator itr = this->controls().begin(); itr != this->controls().end(); ++itr)
		childHeight += (*itr)->height() + this->padding();

	return childHeight;
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

void Container::setPosition(float x, float y)
{
	Control::setPosition(x, y);
	this->updateChildControls();
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




/*************************************************************************/
/** Scrollbar															**/
/*************************************************************************/
Scrollbar::Scrollbar(Control* container)
	: mControl(container), mScroll(0), mWidth(7)
{
}

Scrollbar::~Scrollbar()
{

}

float Scrollbar::width()
{
	if (this->mControl->clientHeight() > this->mControl->height())
		return this->mWidth;

	return 0;
}

void Scrollbar::renderScrollbar()
{
	float clientHeight = this->mControl->clientHeight();
	if (clientHeight > this->mControl->height())
	{
		float h = fmax(clientHeight, this->mControl->height());
		float sbh = this->mControl->height() / h;

		glColor3f(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f);
		glBegin(GL_QUADS);
		glVertex2f(this->mControl->box().hitbox[0]+this->mControl->box().hitbox[2]-10, this->mControl->box().hitbox[1]+this->mControl->box().hitbox[3] - 4 - (this->mScroll*sbh));
		glVertex2f(this->mControl->box().hitbox[0]+this->mControl->box().hitbox[2]-4, this->mControl->box().hitbox[1]+this->mControl->box().hitbox[3] - 4 - (this->mScroll*sbh));
		glVertex2f(this->mControl->box().hitbox[0]+this->mControl->box().hitbox[2]-4, this->mControl->box().hitbox[1]+this->mControl->box().hitbox[3] - ((this->mControl->box().hitbox[3] - 8 + this->mScroll)*sbh));
		glVertex2f(this->mControl->box().hitbox[0]+this->mControl->box().hitbox[2]-10, this->mControl->box().hitbox[1]+this->mControl->box().hitbox[3] - ((this->mControl->box().hitbox[3] - 8 + this->mScroll)*sbh));
		glEnd();
	}
}

void Scrollbar::scrollUp()
{
	float clientHeight = this->mControl->clientHeight();
	if (clientHeight > this->mControl->height())
	{
		this->mScroll += 5.0f;
		float diff = clientHeight - this->mControl->height();
		if (this->mScroll > diff)
			this->mScroll = diff;
	}
	else if (this->mControl->parent() != 0)
	{
		this->mControl->parent()->scrollbar.scrollUp();
	}
}

void Scrollbar::scrollDown()
{
	if (this->mControl->clientHeight() > this->mControl->height())
	{
		this->mScroll -= 5.0f;
		if (this->mScroll < 0)
			this->mScroll = 0;
	}
	else if (this->mControl->parent() != 0)
	{
		this->mControl->parent()->scrollbar.scrollDown();
	}
}

float Scrollbar::scroll()
{
	return this->mScroll;
}

float Scrollbar::globalScroll()
{
	if (this->mControl->parent() != 0)
		return this->mControl->parent()->scrollbar.globalScroll() + this->mScroll;
	return this->mScroll;
}

}
