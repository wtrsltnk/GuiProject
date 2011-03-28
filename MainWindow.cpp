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
	ui::Manager::destroyInstance();
}

void MainWindow::anderenaam(ui::Control* sender, event::EventArgs* e)
{
	if (cb != 0)
	{
		cb->setSize(cb->width(), cb->height()+4);
		cb->StateChanged -= ui::StateChangedEventHandler(this, (ui::StateChangedEvent::FunctionPtr)&MainWindow::anderenaam2);
	}
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

	ui::Button* b = new ui::Button(10, 10, 64, 24, (const char*)"FDfgsdf");
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
	ui::VerticalContainer* cnt = new ui::VerticalContainer(120, 50, 72, 300);
	ui::VerticalContainer* cnt2 = new ui::VerticalContainer(210, 50, 200, 350);
	cnt->addControl(cb);
	cnt->addControl(tb);
	cnt->addControl(lb);
	cnt->addControl(b);
	cnt->addControl(lbl);
	cnt->addControl(vb);
	cnt2->addControl(cnt);

	// Voorbeeld van het toevoegen van events
	b->Click += ui::ClickEventHandler(this, (ui::ClickEvent::FunctionPtr)&MainWindow::anderenaam);
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
}
