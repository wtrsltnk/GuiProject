/*
 * MainWindow.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#include "MainWindow.h"
#include "geo/MapLoader.h"
#include <stdio.h>

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

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

void MainWindow::onMouseMoveEvent(ui::Control* sender, ui::MouseButtonEventArgs* e)
{
	static ui::MouseState lastState = e->state;
	if (e->state.isButtonPressed(ui::Mouse::Left))
	{
		float deltax = e->state.getMousePositionX() - lastState.getMousePositionX();
		float deltay = lastState.getMousePositionY() - e->state.getMousePositionY();
		this->mCamera.rotate(Deg2Rad(deltay/10.0f), 0, Deg2Rad(deltax/10.0f));
	}
	lastState = e->state;
}

void MainWindow::onMouseDownEvent(ui::Control* sender, ui::MouseButtonEventArgs* e)
{
	static ui::MouseState lastState = e->state;

	if (e->button == ui::Mouse::Left)
	{
		// Left mouse button was just pressed
		printf("Left mouse button was just pressed\n");
	}
}

void MainWindow::render3D(ui::Control* sender, event::EventArgs* e)
{
	float speed = 0.2f;
	if (ui::KeyboardState::currentState().isKeyPressed(ui::Key::W) || ui::KeyboardState::currentState().isKeyPressed(ui::Key::w))
		this->mCamera.moveForward(speed);
	if (ui::KeyboardState::currentState().isKeyPressed(ui::Key::S) || ui::KeyboardState::currentState().isKeyPressed(ui::Key::s))
		this->mCamera.moveForward(-speed);
	if (ui::KeyboardState::currentState().isKeyPressed(ui::Key::A) || ui::KeyboardState::currentState().isKeyPressed(ui::Key::a))
		this->mCamera.moveLeft(speed);
	if (ui::KeyboardState::currentState().isKeyPressed(ui::Key::D) || ui::KeyboardState::currentState().isKeyPressed(ui::Key::d))
		this->mCamera.moveLeft(-speed);

	this->mCamera.update();
	glTranslatef(0, 0, -20.0f);
	glScalef(0.01f, 0.01f, 0.01f);

	for(std::vector<geo::Entity*>::iterator e = this->scene.mEntities.begin(); e != this->scene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			this->renderBrushVertices(*(*b));
		}
	}

}

bool MainWindow::initialize(int argc, char* argv[])
{
	geo::MapLoader loader;
	if (loader.load("dust_001.map", &this->scene) == false)
		printf("Load failed\n");
	
	ui::Manager::createInstance("Ubuntu-R.ttf");

	vb = new ui::Valuebox(10, 180, 64, 24, 5, 0, 10000);
	vbx = new ui::Valuebox(10, 180, 64, 24, 5, 0, 360);
	vby = new ui::Valuebox(10, 180, 64, 24, 5, 0, 360);
	ui::Render3D* td = new ui::Render3D(0, 0, 500, 500);
	ui::Container* cnt = new ui::VerticalContainer(0, 0, 100, 500);
	ui::Container* cnt2 = new ui::HorizontalContainer(40, 0, 600, 500);
	cnt->addControl(vb);
	cnt->addControl(vbx);
	cnt->addControl(vby);
	cnt2->addControl(cnt);
	cnt2->addControl(td);

	// Voorbeeld van het toevoegen van events
	td->onRender += ui::Render3DEventHandler(this, (ui::Render3DEvent::FunctionPtr)&MainWindow::render3D);
	td->onMouseMove += ui::MouseButtonEventHandler(this, (ui::MouseEvent::FunctionPtr)&MainWindow::onMouseMoveEvent);
	td->onMouseButtonDown += ui::MouseButtonEventHandler(this, (ui::MouseEvent::FunctionPtr)&MainWindow::onMouseDownEvent);

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

void MainWindow::render(int time)
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
	static int lastTime = 0;
	float speed = 1.0f * ((time - lastTime) / 100.0f);
	lastTime = time;

	glPopMatrix();
}

void MainWindow::renderBrushVertices(geo::Brush& brush)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1,1,1);
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
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(std::vector<Vector3>::iterator itr = brush.mVertices.begin(); itr != brush.mVertices.end(); ++itr)
	{
		glVertex3fv((*itr));
	}
	glEnd();
}
