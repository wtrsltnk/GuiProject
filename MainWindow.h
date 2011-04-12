/*
 * MainWindow.h
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "camera.h"


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

	virtual void render3D(ui::Control* sender, event::EventArgs* e);

	virtual void onMouseEvent(ui::Control* sender, ui::MouseButtonEventArgs* e);

	virtual bool initialize(int argc, char* argv[]);
	virtual void resize(int w, int h);
	virtual void onKeyboard(unsigned char key, int x, int y);
	virtual void onKeyboardUp(unsigned char key, int x, int y);
	virtual void onSpecialKeyboard(int key, int x, int y);
	virtual void onSpecialKeyboardUp(int key, int x, int y);
	virtual void onMouseClick(int button, int state, int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void render(int time);

	void renderBrushVertices(geo::Brush& brush);

	Camera mCamera;
	ui::Valuebox* vb;
	ui::Valuebox* vbx;
	ui::Valuebox* vby;

	geo::Brush brush;
	geo::Scene scene;
	int index1, index2;
//	Video video;

};

#endif /* MAINWINDOW_H_ */
