/*
 * GuiProject.h
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#ifndef GUIPROJECT_H_
#define GUIPROJECT_H_

#include "GlutApplication.h"
#include "common/camera.h"
#include "ui/uiControls.h"
#include "geo/Brush.h"
#include "geo/Scene.h"
#include "Video.h"

class MainWindow : public GlutApplication, public event::EventHandler
{
public:
	MainWindow();
	virtual ~MainWindow();

	virtual void render3D(ui::Control* sender, event::EventArgs* e);

	virtual void onMouseMoveEvent(ui::Control* sender, ui::MouseButtonEventArgs* e);
	virtual void onMouseDownEvent(ui::Control* sender, ui::MouseButtonEventArgs* e);

	virtual bool initialize(int argc, char* argv[]);
	virtual void resize(int w, int h);
	virtual void onKeyboardDown(Key::Code key) { }
	virtual void onKeyboardUp(Key::Code key) { }
	virtual void onMouseDown(Mouse::Button button) { }
	virtual void onMouseUp(Mouse::Button button) { }
	virtual void onMouseMove(int x, int y) { }
	virtual void render(int time);

	void renderBrushVertices(geo::Brush& brush);

	Camera mCamera;
	ui::Valuebox* vb;
	ui::Valuebox* vbx;
	ui::Valuebox* vby;

	geo::Brush brush;
	geo::Scene scene;
	int index1, index2;
	Video video;

};

#endif /* GUIPROJECT_H_ */
