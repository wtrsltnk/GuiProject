#include "uiControls.h"
#include "uiFont.h"
#include "uiClipper.h"

namespace ui
{

Listbox::Listbox(int x, int y, int w, int h)
	: Control(ControlTypes::Listbox, x, y, w, h), scrollbar(Scrollbar(this)), SelectedIndexChanged(SelectedIndexChangedEvent(this)), mPadding(4), mSelectedIndex(-1)
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

	glColor3f(0.3f, 0.7f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(hitbox[0], hitbox[1]);
	glVertex2f(hitbox[0]+hitbox[2], hitbox[1]);
	glVertex2f(hitbox[0]+hitbox[2], hitbox[1]+hitbox[3]);
	glVertex2f(hitbox[0], hitbox[1]+hitbox[3]);
	glEnd();

	float y = 20;
	glPushMatrix();
	glTranslatef(0, this->scrollbar.scroll(), 0);
	int i = 0;
	for (std::vector<ListboxItem>::iterator itr = this->mItems.begin(); itr != this->mItems.end(); ++itr)
	{
		unsigned int color = RGBA(0, 0, 0, 255);
		if (this->mSelectedIndex == i++)
			color = RGBA(255, 255, 0, 255);
		Control::renderText(
				this->mBox.hitbox[0] + this->mPadding,
				this->mBox.hitbox[1] + this->mBox.hitbox[3] - y,
				(*itr).mText, color);

		glColor3f(0.1f, 0.5f, 0.8f);
		glBegin(GL_LINES);
		glVertex2f(this->mBox.hitbox[0], this->mBox.hitbox[1] + this->mBox.hitbox[3] - y - this->mPadding);
		glVertex2f(this->mBox.hitbox[0]+this->mBox.hitbox[2], this->mBox.hitbox[1] + this->mBox.hitbox[3] - y - this->mPadding);
		glEnd();

		y += 20 + this->mPadding;
	}
	glPopMatrix();
}

void Listbox::mouseDown(int button, int x, int y)
{
	if (button == 4)
		this->scrollbar.scrollUp();
	else if (button == 3)
		this->scrollbar.scrollDown();
	else if (button == 0)
	{
		int localY = -int(y + this->mPadding - this->scrollbar.scroll() - (this->mBox.hitbox[1]+this->mBox.hitbox[3]));
		this->setSelectedIndex(localY / (20+int(this->mPadding)));
	}
}

float Listbox::clientHeight()
{
	float clientHeight = this->mPadding;
	for (std::vector<ListboxItem>::iterator itr = this->mItems.begin(); itr != this->mItems.end(); ++itr)
	{
		clientHeight += 20;
		clientHeight += this->mPadding;
	}
	return clientHeight;
}

void Listbox::addItem(const char* text, void* data)
{
	this->mItems.push_back(ListboxItem(text, data));
}

int Listbox::selectedIndex() const
{
	return this->mSelectedIndex;
}

void Listbox::setSelectedIndex(int index)
{
	if (this->mSelectedIndex != index)
	{
		SelectedIndexChangedEventArgs e(this->mSelectedIndex, index);
		this->mSelectedIndex = index;
		this->SelectedIndexChanged(&e);
	}
}

Listbox::ListboxItem Listbox::selectedItem() const
{
	if (this->mSelectedIndex >= 0 && this->mSelectedIndex < int(this->mItems.size()))
		return this->mItems[this->mSelectedIndex];
	return Listbox::ListboxItem("", 0);
}

Listbox::ListboxItem::ListboxItem(const char* text, void* data)
	: mText(text), mData(data)
{
}

SelectedIndexChangedEventArgs::SelectedIndexChangedEventArgs(int previousIndex, int newIndex)
	: mPreviousIndex(previousIndex), mNewIndex(newIndex)
{
}

SelectedIndexChangedEventArgs::~SelectedIndexChangedEventArgs()
{
}

int SelectedIndexChangedEventArgs::previousIndex() const
{
	return this->mPreviousIndex;
}

int SelectedIndexChangedEventArgs::newIndex() const
{
	return this->mNewIndex;
}

}
