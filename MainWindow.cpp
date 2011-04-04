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
	: GlutApplication("GuiProject"), rot(0), rot2(0)
{
	this->brush.mPlanes.push_back(geo::Plane(geo::Vertex(1, 0, 0), geo::Vertex(0, 0, 0), geo::Vertex(0, 1, 0)));
	this->brush.mPlanes.push_back(geo::Plane(geo::Vertex(1, 0, 0), geo::Vertex(0, 0, 1), geo::Vertex(0, 0, 0)));
	this->brush.mPlanes.push_back(geo::Plane(geo::Vertex(0, 0, 1), geo::Vertex(0, 1, 0), geo::Vertex(0, 0, 0)));
	this->brush.mPlanes.push_back(geo::Plane(geo::Vertex(0, 1, 1), geo::Vertex(1, 0, 1), geo::Vertex(1, 1, 1)));
	this->brush.mPlanes.push_back(geo::Plane(geo::Vertex(0, 1, 1), geo::Vertex(1, 1, 1), geo::Vertex(1, 1, 0)));
	this->brush.mPlanes.push_back(geo::Plane(geo::Vertex(1, 1, 0), geo::Vertex(1, 1, 1), geo::Vertex(1, 0, 1)));
}

MainWindow::~MainWindow()
{
	ui::Manager::destroyInstance();
}

void MainWindow::rechtsomDraaien(ui::Control* sender, event::EventArgs* e)
{
	rot += vb->value();
}

void MainWindow::linksomDraaien(ui::Control* sender, event::EventArgs* e)
{
	rot -= vb->value();
}

void MainWindow::omhoogDraaien(ui::Control* sender, event::EventArgs* e)
{
	rot2 += vb->value();
}

void MainWindow::omlaagDraaien(ui::Control* sender, event::EventArgs* e)
{
	rot2 -= vb->value();
}

bool MainWindow::initialize()
{
	geo::MapLoader loader;
	if (loader.load("qtest.map", &this->scene) == false)
		printf("Load failed\n");
	
	ui::Manager::createInstance("Ubuntu-R.ttf");

	ui::Button* b1 = new ui::Button(10, 10, 64, 24, (const char*)"left");
	ui::Button* b2 = new ui::Button(10, 10, 64, 24, (const char*)"right");
	ui::Button* b3 = new ui::Button(10, 10, 64, 24, (const char*)"up");
	ui::Button* b4 = new ui::Button(10, 10, 64, 24, (const char*)"down");
	vb = new ui::Valuebox(10, 180, 64, 24, 5, 0, 10000);
	ui::Container* cnt = new ui::VerticalContainer(30, 30, 100, 500);
	cnt->addControl(b1);
	cnt->addControl(b2);
	cnt->addControl(b3);
	cnt->addControl(b4);
	cnt->addControl(vb);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPushMatrix();
	glTranslatef(0, 0, -10.0f);
	glRotatef(rot, 0, 1, 0);
	glRotatef(rot2, 1, 0, 0);
	glScalef(0.01f, 0.01f, 0.01f);

	for(std::vector<geo::Entity*>::iterator e = this->scene.mEntities.begin(); e != this->scene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			this->renderBrush(*(*b));
		}
	}
	glPopMatrix();
}

void MainWindow::renderBrush(geo::Brush& brush)
{
	glColor3f(0.6f, 0.6f, 0.6f);
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for(std::vector<geo::Plane>::iterator itr = brush.mPlanes.begin(); itr != brush.mPlanes.end(); ++itr)
	{
		glVertex3fv((*itr).mBase[0]);
		glVertex3fv((*itr).mBase[1]);
		glVertex3fv((*itr).mBase[2]);
	}
	glEnd();

	glColor3f(0.1f, 0.1f, 0.1f);
	glCullFace(GL_BACK);
	glBegin(GL_TRIANGLES);
	for(std::vector<geo::Plane>::iterator itr = brush.mPlanes.begin(); itr != brush.mPlanes.end(); ++itr)
	{
		glVertex3fv((*itr).mBase[0]);
		glVertex3fv((*itr).mBase[1]);
		glVertex3fv((*itr).mBase[2]);
	}
	glEnd();

	glDisable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for(std::vector<geo::Plane>::iterator itr = brush.mPlanes.begin(); itr != brush.mPlanes.end(); ++itr)
	{
		glVertex3fv((*itr).mBase[0]);
		glVertex3fv((*itr).mBase[1]);
		glVertex3fv((*itr).mBase[2]);
	}
	glEnd();

	glPointSize(5);
	glColor3f(0, 0.5f, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glBegin(GL_POINTS);
	for(std::vector<geo::Plane>::iterator itr = brush.mPlanes.begin(); itr != brush.mPlanes.end(); ++itr)
	{
		glVertex3fv((*itr).mBase[0]);
		glVertex3fv((*itr).mBase[1]);
		glVertex3fv((*itr).mBase[2]);
	}
	glEnd();
}
