/*
 * MainWindow.h
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "GlutApplication.h"
#include "ui/uiControls.h"
#include "geo/Brush.h"
#include "geo/Scene.h"
//#include "Video.h"

class MainWindow : public GlutApplication, public event::EventHandler
{
public:
	MainWindow();
	virtual ~MainWindow();

	virtual void rechtsomDraaien(ui::Control* sender, event::EventArgs* e);
	virtual void linksomDraaien(ui::Control* sender, event::EventArgs* e);
	virtual void omhoogDraaien(ui::Control* sender, event::EventArgs* e);
	virtual void omlaagDraaien(ui::Control* sender, event::EventArgs* e);

	virtual bool initialize();
	virtual void resize(int w, int h);
	virtual void onKeyboard(unsigned char key, int x, int y) { }
	virtual void onSpecialKeyboard(int key, int x, int y) { }
	virtual void onMouseClick(int button, int state, int x, int y) { }
	virtual void onMouseMove(int x, int y) { }
	virtual void render();

	void renderBrush(geo::Brush& brush);
	ui::Valuebox* vb;

	geo::Brush brush;
	geo::Scene scene;
	float rot;
	float rot2;
//	Video video;

};

#endif /* MAINWINDOW_H_ */
