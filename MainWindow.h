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
//#include "Video.h"

class MainWindow : public GlutApplication, public EventHandler
{
public:
	MainWindow();
	virtual ~MainWindow();

	virtual void anderenaam(void* sender, EventArgs* e);
	virtual void anderenaam2(void* sender, EventArgs* e);
	virtual void anderenaam3(void* sender, EventArgs* e);

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
//	Video video;

};

#endif /* MAINWINDOW_H_ */
