/*
 * MainWindow.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#include "MainWindow.h"
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

void MainWindow::anderenaam2(ui::Control* sender, event::EventArgs* e)
{
	printf("hellow world\n");
}

void MainWindow::anderenaam3(ui::Control* sender, event::EventArgs* e)
{
	ui::Textbox* tb = (ui::Textbox*)sender;
	lbl->setText(tb->text());

}

void MainWindow::anderenaam4(ui::Control* sender, ui::SelectedIndexChangedEventArgs* e)
{
	printf("%d %d %s\n", e->previousIndex(), e->newIndex(), ((ui::Listbox*)sender)->selectedItem().text());
}

bool MainWindow::initialize()
{
	ui::Manager::createInstance("Ubuntu-R.ttf");

	ui::Button* b = new ui::Button(10, 10, 64, 24, (const char*)"left");
	ui::Button* b2 = new ui::Button(10, 10, 64, 24, (const char*)"right");
	ui::Button* b3 = new ui::Button(10, 10, 64, 24, (const char*)"up");
	ui::Button* b4 = new ui::Button(10, 10, 64, 24, (const char*)"down");
	this->cb = new ui::Checkbox(10, 80, 64, 54, (const char*)"asdfsd sdfasdfa dsfasdasd");
	tb = new ui::Textbox(10, 110, 164, 54, (const char*)"Wouter Saaltink");
	vb = new ui::Valuebox(10, 180, 64, 24, 5.8346, 0, 10000);
	lbl = new ui::Label("test", 10, 440, 164, 24);
	ui::Listbox* lb = new ui::Listbox(200, 64, 20, 100);
	lb->addItem("Wouter");
	lb->addItem("Is");
	lb->addItem("Heel");
	lb->addItem("Handig");
	lb->addItem("Met c++");
	lb->addItem("Wouter");
	lb->addItem("Is");
	lb->addItem("Heel");
	lb->addItem("Handig");
	lb->addItem("Met c++");
	lb->SelectedIndexChanged += ui::SelectedIndexChangedEventHandler(this, (ui::SelectedIndexChangedEvent::FunctionPtr)&MainWindow::anderenaam4);
	ui::Container* cnt = new ui::VerticalContainer(120, 50, 100, 200);
	cnt->addControl(b);
	cnt->addControl(b2);
	cnt->addControl(b3);
	cnt->addControl(b4);
	cnt->addControl(vb);
	cnt->addControl(cb);
	cnt->addControl(tb);
	cnt->addControl(lb);
	cnt->addControl(lbl);
	//cnt2->addControl(cnt);

	// Voorbeeld van het toevoegen van events
	b->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::linksomDraaien);
	b2->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::rechtsomDraaien);
	b3->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::omhoogDraaien);
	b4->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::omlaagDraaien);
	cb->StateChanged += ui::StateChangedEventHandler(this, (ui::StateChangedEvent::FunctionPtr)&MainWindow::anderenaam2);
	tb->TextChanged += ui::TextChangedEventHandler(this, (ui::TextChangedEvent::FunctionPtr)&MainWindow::anderenaam3);

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

	glTranslatef(0, 0, -10.0f);
	glRotatef(rot, 0, 1, 0);
	glRotatef(rot2, 1, 0, 0);

	glColor3f(0.1f, 0.1f, 0.1f);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for(std::vector<geo::Plane>::iterator itr = brush.mPlanes.begin(); itr != brush.mPlanes.end(); ++itr)
	{
		glVertex3fv((*itr).mBase[0]);
		glVertex3fv((*itr).mBase[1]);
		glVertex3fv((*itr).mBase[2]);
	}
	glEnd();

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

	glDisable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);
	glCullFace(GL_BACK);
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
	glCullFace(GL_BACK);
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
