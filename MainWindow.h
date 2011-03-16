/*
 * MainWindow.h
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "GlutApplication.h"
#include "Font.h"
#include "GuiManager.h"
#include "Controls.h"

class MainWindow : public GlutApplication, public ui::GuiEventHandler
{
public:
	MainWindow();
	virtual ~MainWindow();

	virtual void anderenaam(ui::Control* box, ui::EventArgs* e);
	virtual void anderenaam2(ui::Control* box, ui::EventArgs* e);
	virtual void anderenaam3(ui::Control* box, ui::EventArgs* e);

	virtual bool initialize();
	virtual void resize(int w, int h);
	virtual void onKeyboard(unsigned char key, int x, int y) { }
	virtual void onSpecialKeyboard(int key, int x, int y) { }
	virtual void onMouseClick(int button, int state, int x, int y) { }
	virtual void onMouseMove(int x, int y) { }
	virtual void render();

	ui::Checkbox* cb;
	ui::Textbox* tb;
	ui::Valuebox* vb;
	ui::Label* lbl;

};

#endif /* MAINWINDOW_H_ */
