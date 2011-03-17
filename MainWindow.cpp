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
	: GlutApplication("GuiProject")
{
}

MainWindow::~MainWindow()
{
	ui::GuiManager::destroyInstance();
}

void MainWindow::anderenaam(ui::Control* box, ui::EventArgs* e)
{
	printf("Button 1\n");

	if (cb != 0)
	{
		float pos[2];
		cb->size(pos);
		pos[1] += 4;
		cb->setSize(pos);
	}
}

void MainWindow::anderenaam2(ui::Control* box, ui::EventArgs* e)
{
	float size[2];
	tb->size(size);
	if (((ui::Checkbox*)box)->checked())
		tb->setSize(size[0], size[1]+10);
	else
		tb->setSize(size[0], size[1]-10);
}

void MainWindow::anderenaam3(ui::Control* box, ui::EventArgs* e)
{
	ui::Textbox* tb = (ui::Textbox*)box;
	printf("%s\n", tb->text());
	lbl->setText(tb->text());
}

bool MainWindow::initialize()
{
	ui::GuiManager::createInstance("Ubuntu-R.ttf");

	ui::Button* b = new ui::Button(10, 10, 64, 24, (const char*)"FDfgsdf");
	cb = new ui::Checkbox(10, 80, 64, 24, (const char*)"asdfasd");
	tb = new ui::Textbox(10, 110, 164, 54, (const char*)"Wouters");
	vb = new ui::Valuebox(10, 180, 64, 24, 5.8346, 0, 10000);
	lbl = new ui::Label("test", 10, 440, 164, 24);
	ui::Container* cnt = new ui::Container(120, 10, 72, 100);
	cnt->addControl(cb);
	cnt->addControl(tb);

	// Voorbeeld van het toevoegen van events
	b->Click += ui::ClickEventHandler(this, (ui::eventFn)&MainWindow::anderenaam);
	cb->StateChanged += ui::StateChangedEventHandler(this, (ui::eventFn)&MainWindow::anderenaam2);
	tb->TextChanged += ui::TextChangedEventHandler(this, (ui::eventFn)&MainWindow::anderenaam3);

	glClearColor(62.0f / 255.0f, 62.0f / 255.0f, 62.0f / 255.0f, 1.0f);

	this->video.load("/media/data/Films/tron/Tron.Legacy.2010.x264.1080.BluRay.AC3-DTS.NLSUBS-SRT/Tron.Legacy.2010.x264.1080.BluRay.AC3-DTS.NLSUBS-SRT.mkv");

	return true;
}

void MainWindow::resize(int w, int h)
{
	this->width = w;
	glViewport(0, 0, w, h);
	ui::GuiManager::instance()->setupSize(w, h);
}

void MainWindow::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->video.nextFrame();
	this->video.render();

	ui::GuiManager::instance()->render();
}
