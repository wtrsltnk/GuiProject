#include "uiControls.h"
#include "uiFont.h"

namespace ui
{

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

void Checkbox::mouseDown(int button, int x, int y)
{
	if (button == 0)
		this->toggleChecked();
	Control::mouseDown(button, x, y);
}

bool Checkbox::checked()
{
	return this->mChecked;
}

void Checkbox::setChecked(bool state)
{
	this->mChecked = state;
	event::EventArgs e;
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

}
