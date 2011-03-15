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
		// Voorbeeld van het weghalen van een event
		cb->StateChanged -= ui::StateChangedEventHandler(this, (ui::eventFn)&MainWindow::anderenaam2);

		// Voorbeeld waarin de Handler op zichzelf ook aanroepbaar is
		ui::StateChangedEventHandler h(this, (ui::eventFn)&MainWindow::anderenaam2);
		h(cb, e);
		delete cb;
		cb=0;
	}
}

void MainWindow::anderenaam2(ui::Control* box, ui::EventArgs* e)
{
	printf("Button 2\n");
}

bool MainWindow::initialize()
{
	ui::GuiManager::createInstance("Ubuntu-R.ttf");

	ui::Button* b = new ui::Button(10, 10, 64, 24, (const char*)"FDfgsdf");
	cb = new ui::Checkbox(10, 80, 64, 24, (const char*)"asdfasd");
	new ui::Textbox(10, 110, 164, 54, (const char*)"Wouters");
	new ui::Valuebox(10, 180, 64, 24, 5.8346, 0, 10000);

	// Voorbeeld van het toevoegen van events
	b->Click += ui::ClickEventHandler(this, (ui::eventFn)&MainWindow::anderenaam);
	cb->StateChanged += ui::StateChangedEventHandler(this, (ui::eventFn)&MainWindow::anderenaam2);

	glClearColor(62.0f / 255.0f, 62.0f / 255.0f, 62.0f / 255.0f, 1.0f);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ui::GuiManager::instance()->render();
}
