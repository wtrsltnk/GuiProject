/*
 * MainWindow.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#include "MainWindow.h"
#include "geo/MapLoader.h"
#include <stdio.h>

GlutApplication* gApplication = new MainWindow();

MainWindow::MainWindow()
	: GlutApplication("GuiProject"), index1(0), index2(0)
{
	this->brush.mPlanes.push_back(geo::Plane::fromVertices(Vector3(1, 0, 0), Vector3(0, 0, 0), Vector3(0, 1, 0)));
	this->brush.mPlanes.push_back(geo::Plane::fromVertices(Vector3(1, 0, 0), Vector3(0, 0, 1), Vector3(0, 0, 0)));
	this->brush.mPlanes.push_back(geo::Plane::fromVertices(Vector3(0, 0, 1), Vector3(0, 1, 0), Vector3(0, 0, 0)));
	this->brush.mPlanes.push_back(geo::Plane::fromVertices(Vector3(0, 1, 1), Vector3(1, 0, 1), Vector3(1, 1, 1)));
	this->brush.mPlanes.push_back(geo::Plane::fromVertices(Vector3(0, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 0)));
	this->brush.mPlanes.push_back(geo::Plane::fromVertices(Vector3(1, 1, 0), Vector3(1, 1, 1), Vector3(1, 0, 1)));
}

MainWindow::~MainWindow()
{
	ui::Manager::destroyInstance();
}

void MainWindow::rechtsomDraaien(ui::Control* sender, event::EventArgs* e)
{
	index1--;
	if (index1 < 0) index1 = 0;
	char str[256] = { 0 };
	sprintf(str, "Brush : %d, Plane : %d", index1, index2);
	lbl->setText(str);
}

void MainWindow::linksomDraaien(ui::Control* sender, event::EventArgs* e)
{
	index1++;
	char str[256] = { 0 };
	sprintf(str, "Brush : %d, Plane : %d", index1, index2);
	lbl->setText(str);
}

void MainWindow::omhoogDraaien(ui::Control* sender, event::EventArgs* e)
{
	index2++;
	char str[256] = { 0 };
	sprintf(str, "Brush : %d, Plane : %d", index1, index2);
	lbl->setText(str);
}

void MainWindow::omlaagDraaien(ui::Control* sender, event::EventArgs* e)
{
	index2--;
	if (index2 < 0) index2 = 0;
	char str[256] = { 0 };
	sprintf(str, "Brush : %d, Plane : %d", index1, index2);
	lbl->setText(str);
}

void MainWindow::onSpecialKeyboard(int key, int x, int y)
{
}

bool MainWindow::initialize()
{
	geo::MapLoader loader;
	if (loader.load("dust_001.map", &this->scene) == false)
		printf("Load failed\n");
	
	ui::Manager::createInstance("Ubuntu-R.ttf");

	ui::Button* b1 = new ui::Button(10, 10, 64, 24, (const char*)"Next Brush");
	ui::Button* b2 = new ui::Button(10, 10, 64, 24, (const char*)"Prev Brush");
	ui::Button* b3 = new ui::Button(10, 10, 64, 24, (const char*)"Next Plane");
	ui::Button* b4 = new ui::Button(10, 10, 64, 24, (const char*)"Prev Plane");
	vb = new ui::Valuebox(10, 180, 64, 24, 5, 0, 10000);
	vbx = new ui::Valuebox(10, 180, 64, 24, 5, 0, 360);
	vby = new ui::Valuebox(10, 180, 64, 24, 5, 0, 360);
	lbl = new ui::Label("Brush : 0, Plane : 0", 0, 0, 200, 200);
	ui::Container* cnt = new ui::VerticalContainer(30, 30, 200, 500);
	cnt->addControl(b1);
	cnt->addControl(b2);
	cnt->addControl(b3);
	cnt->addControl(b4);
	cnt->addControl(vb);
	cnt->addControl(vbx);
	cnt->addControl(vby);
	cnt->addControl(lbl);

	// Voorbeeld van het toevoegen van events
	b1->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::linksomDraaien);
	b2->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::rechtsomDraaien);
	b3->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::omhoogDraaien);
	b4->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::omlaagDraaien);

	glClearColor(62.0f / 255.0f, 62.0f / 255.0f, 62.0f / 255.0f, 1.0f);

//	this->video.load("/media/data/Films/tron/Tron.Legacy.2010.x264.1080.BluRay.AC3-DTS.NLSUBS-SRT/Tron.Legacy.2010.x264.1080.BluRay.AC3-DTS.NLSUBS-SRT.mkv");

	return true;
}

void MainWindow::resize(int w, int h)
{
	this->width = w;
	glViewport(0, 0, w, h);
	ui::Manager::instance()->setupSize(w, h);
}

void MainWindow::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//	if (this->cb->checked())
//		this->video.nextFrame();
//	this->video.render();

	ui::Manager::instance()->render();

	float aspect = 1.0f * ((float)this->width/(float)this->height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, aspect, 0.1f, 3000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -20.0f);
	glRotatef(this->vbx->value(), 0, 1, 0);
	glRotatef(this->vby->value(), 1, 0, 0);
	glScalef(0.01f, 0.01f, 0.01f);
/*
	if (this->index1 < this->scene.mEntities[0]->mBrushes.size())
	{
		geo::Brush* brush = this->scene.mEntities[0]->mBrushes[this->index1];
		if (this->index2 < brush->mPlanes.size())
		{
			geo::Plane& plane = brush->mPlanes[this->index2];

			glColor3fv(plane.mColor);
			glBegin(GL_LINE_STRIP);
			for(std::vector<int>::iterator itr = plane.mIndices.begin(); itr != plane.mIndices.end(); ++itr)
			{
				glVertex3f(brush->mVertices[(*itr)][0], brush->mVertices[(*itr)][1], brush->mVertices[(*itr)][2]);
			}
			glEnd();
			glPointSize(4);
			glColor3f(1, 0, 0);
			glBegin(GL_POINTS);
			glVertex3fv(brush->mVertices[plane.mIndices[0]]);
			glVertex3fv(plane.average);
			glEnd();
		}
	}
/*/
	for(std::vector<geo::Entity*>::iterator e = this->scene.mEntities.begin(); e != this->scene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			this->renderBrushVertices(*(*b));
		}
	}
// */
	glPopMatrix();
}

void MainWindow::renderBrushVertices(geo::Brush& brush)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3fv(brush.mColor);
	for(std::vector<geo::Plane>::iterator p = brush.mPlanes.begin(); p != brush.mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
		{
			glVertex3f(brush.mVertices[(*itr)][0], brush.mVertices[(*itr)][1], brush.mVertices[(*itr)][2]);
		}
		glEnd();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1,1,1);
	for(std::vector<geo::Plane>::iterator p = brush.mPlanes.begin(); p != brush.mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
		{
			glVertex3f(brush.mVertices[(*itr)][0], brush.mVertices[(*itr)][1], brush.mVertices[(*itr)][2]);
		}
		glEnd();
	}
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(std::vector<Vector3>::iterator itr = brush.mVertices.begin(); itr != brush.mVertices.end(); ++itr)
	{
		glVertex3fv((*itr));
	}
	glEnd();
}
