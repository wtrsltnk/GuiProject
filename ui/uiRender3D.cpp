#include "uiControls.h"
#include <GL/gl.h>
#include <GL/glu.h>

namespace ui
{

Render3D::Render3D(int x, int y, int w, int h)
	: Control(ControlTypes::Render3D, x, y, w, h), onRender(Render3DEvent(this))
{
}

Render3D::~Render3D()
{
}

void Render3D::render()
{
	this->renderBox(false);
	float aspect = 1.0f * ((float)this->width()/(float)this->height());

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPerspective(45.0f, aspect, 0.1f, 3000.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	event::EventArgs e;
	this->onRender(&e);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

}
