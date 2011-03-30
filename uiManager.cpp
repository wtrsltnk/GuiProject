/*
 * uiManager.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#include "uiManager.h"
#include "uiFont.h"
#include "uiControls.h"
#include "MainWindow.h"
#include <GL/freeglut.h>

namespace ui
{

Manager::Manager()
	: mFocus(0), mRoot(0)
{
}

Manager* Manager::sInstance = 0;
Font* Manager::sDefaultFont = 0;

Manager* Manager::createInstance(const char* fontpath)
{
	if (Manager::sInstance != 0)
		delete Manager::sInstance;

	Manager::sInstance = new Manager();
	Manager::sInstance->initialize(fontpath);

	return (Manager*)Manager::sInstance;
}

Manager* Manager::instance()
{
	return Manager::sInstance;
}

void Manager::destroyInstance()
{
	if (Manager::sInstance != 0)
		delete Manager::sInstance;
	Manager::sInstance = 0;
}

Manager::~Manager()
{
	while (this->mControls.empty() == false)
	{
		// Only delete the, since deleting automatically removes them from the GuiManager
		Control* c = this->mControls.back();
		delete c;
	}

	if (this->mRoot != 0)
		delete this->mRoot;
}

void Manager::initialize(const char* fontpath)
{
	if (Manager::sDefaultFont != 0)
		delete Manager::sDefaultFont;

	Manager::sDefaultFont = new Font();
	Manager::sDefaultFont->initializeFont(fontpath);

	Manager::instance()->mRoot = new FreeContainer(20, 20, 10, 10);

#ifndef SKIP_GLUT
	glutKeyboardFunc(&Manager::glutKeyboardDown);
	glutKeyboardUpFunc(&Manager::glutKeyboardUp);
	glutSpecialFunc(&Manager::glutSpecialKeyboardDown);
	glutSpecialUpFunc(&Manager::glutSpecialKeyboardUp);
	glutMouseFunc(&Manager::glutMouseClick);
	glutMotionFunc(&Manager::glutMouseMove);
	glutPassiveMotionFunc(&Manager::glutMouseMove);
#endif
}

void Manager::addControl(Control* ctr)
{
	ctr->updateBox();
	this->mControls.push_back(ctr);
	if (ctr->parent() == 0 && this->mRoot != 0)
		this->mRoot->addControl(ctr);
}

void Manager::removeControl(Control* ctr)
{
	for (std::vector<Control*>::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
	{
		if ((*itr) == ctr)
		{
			this->mControls.erase(itr);
			break;
		}
	}
}

Container* Manager::getRoot()
{
	return this->mRoot;
}

Control* Manager::getTopControlAt(float point[2], Container* container)
{
	Control* result = 0;

	if (container == 0)
		container = Manager::instance()->mRoot;

	if (container != 0)
	{
		for (std::vector<Control*>::iterator itr = container->controls().begin(); itr != container->controls().end(); ++itr)
		{
			Control* c = (*itr);
			if (c->isPointInBox(point))
			{
				result = c;

				Container* cc = dynamic_cast<Container*>(c);
				if (cc != 0)
				{
					Control* tmp = getTopControlAt(point, cc);
					if (tmp != 0)
						result = tmp;
				}

				break;
			}
		}
	}

	return result;
}

void Manager::setupSize(int w, int h)
{
	this->mViewSize[0] = w;
	this->mViewSize[1] = h;
	if (this->mRoot != 0)
		this->mRoot->setSize(w-40, h-40);
}

void Manager::render()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0f, float(this->mViewSize[0]), 0.0f, float(this->mViewSize[1]), -10.0f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	// First make everything writable!
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glColorMask(0, 0, 0, 0);
	glStencilMask(0xFF);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(this->mViewSize[0], 0);
	glVertex2f(this->mViewSize[0], this->mViewSize[1]);
	glVertex2f(0, this->mViewSize[1]);
	glEnd();

	if (this->mRoot != 0)
		this->mRoot->renderControl();

	glDisable(GL_STENCIL_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

#ifndef SKIP_GLUT
void Manager::glutKeyboardDown(unsigned char key, int x, int y)
{
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyDown(Manager::sKeymap[key]);
	}
}
void Manager::glutKeyboardUp(unsigned char key, int x, int y)
{
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyUp(Manager::sKeymap[key]);
	}
}

void Manager::glutSpecialKeyboardDown(int key, int x, int y)
{
	printf("SpecialKey %d\n", key);
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyDown(Manager::sSpecialKeymap[key]);
	}
}

void Manager::glutSpecialKeyboardUp(int key, int x, int y)
{
	if (Manager::instance()->mFocus != 0)
	{
		Manager::instance()->mFocus->keyUp(Manager::sSpecialKeymap[key]);
	}
}

void Manager::glutMouseClick(int button, int state, int x, int y)
{
	float point[2] = { x, Manager::instance()->mViewSize[1] - y };

	Control* control = Manager::instance()->getTopControlAt(point);

	if (control != 0)
	{
		if (state == 0)
		{
			// Only non-containers can get focus
			if (dynamic_cast<Container*>(control) == 0 &&  Manager::sButtonmap[button] == Mouse::Left)
				Manager::instance()->mFocus = control;

			control->mouseDown(Manager::sButtonmap[button]);
		}
		else if (state == 1)
		{
			control->mouseUp(Manager::sButtonmap[button]);
		}
	}
}

void Manager::glutMouseMove(int x, int y)
{
	float point[2] = { x, Manager::instance()->mViewSize[1] - y };

	MouseState::sCurrentState.mMouseX = x;
	MouseState::sCurrentState.mMouseY = Manager::instance()->mViewSize[1] - y;

	static Control* lastHovered = 0;
	Control* control = Manager::instance()->getTopControlAt(point);

	if (control != lastHovered && lastHovered != 0)
		lastHovered->mouseOut();
	if (control != 0)
		control->mouseIn();

	lastHovered = control;
}

Mouse::Button Manager::sButtonmap[]  =
{
/*   0 */	Mouse::Left,
/*   1 */	Mouse::Middle,
/*   2 */	Mouse::Right,
/*   3 */	Mouse::XButton1,
/*   4 */	Mouse::XButton2,
/*   5 */	Mouse::Unknown,
/*   6 */	Mouse::Unknown,
/*   7 */	Mouse::Unknown,
/*   8 */	Mouse::Unknown
};

Key::Code Manager::sKeymap[] =
{
/*   0 */	Key::Unknown,
/*   1 */	Key::Unknown,
/*   2 */	Key::Unknown,
/*   3 */	Key::Unknown,
/*   4 */	Key::Unknown,
/*   5 */	Key::Unknown,
/*   6 */	Key::Unknown,
/*   7 */	Key::Unknown,
/*   8 */	Key::Back,
/*   9 */	Key::Tab,
/*  10 */	Key::Unknown,
/*  11 */	Key::Unknown,
/*  12 */	Key::Unknown,
/*  13 */	Key::Return,
/*  14 */	Key::Unknown,
/*  15 */	Key::Unknown,
/*  16 */	Key::Unknown,
/*  17 */	Key::Unknown,
/*  18 */	Key::Unknown,
/*  19 */	Key::Unknown,
/*  20 */	Key::Unknown,
/*  21 */	Key::Unknown,
/*  22 */	Key::Unknown,
/*  23 */	Key::Unknown,
/*  24 */	Key::Unknown,
/*  25 */	Key::Unknown,
/*  26 */	Key::Unknown,
/*  27 */	Key::Escape,
/*  28 */	Key::Unknown,
/*  29 */	Key::Unknown,
/*  30 */	Key::Unknown,
/*  31 */	Key::Unknown,
/*  32 */	Key::Space,
/*  33 */	Key::Unknown,
/*  34 */	Key::Unknown,
/*  35 */	Key::Unknown,
/*  36 */	Key::Unknown,
/*  37 */	Key::Unknown,
/*  38 */	Key::Unknown,
/*  39 */	Key::Unknown,
/*  40 */	Key::Unknown,
/*  41 */	Key::Unknown,
/*  42 */	Key::Unknown,
/*  43 */	Key::Unknown,
/*  44 */	Key::Unknown,
/*  45 */	Key::Unknown,
/*  46 */	Key::Unknown,
/*  47 */	Key::Unknown,
/*  48 */	Key::Num0,
/*  49 */	Key::Num1,
/*  50 */	Key::Num2,
/*  51 */	Key::Num3,
/*  52 */	Key::Num4,
/*  53 */	Key::Num5,
/*  54 */	Key::Num6,
/*  55 */	Key::Num7,
/*  56 */	Key::Num8,
/*  57 */	Key::Num9,
/*  58 */	Key::Unknown,
/*  59 */	Key::Unknown,
/*  60 */	Key::Unknown,
/*  61 */	Key::Unknown,
/*  62 */	Key::Unknown,
/*  63 */	Key::Unknown,
/*  64 */	Key::Unknown,
/*  65 */	Key::A,
/*  66 */	Key::B,
/*  67 */	Key::C,
/*  68 */	Key::D,
/*  69 */	Key::E,
/*  70 */	Key::F,
/*  71 */	Key::G,
/*  72 */	Key::H,
/*  73 */	Key::I,
/*  74 */	Key::J,
/*  75 */	Key::K,
/*  76 */	Key::L,
/*  77 */	Key::M,
/*  78 */	Key::N,
/*  79 */	Key::O,
/*  80 */	Key::P,
/*  81 */	Key::Q,
/*  82 */	Key::R,
/*  83 */	Key::S,
/*  84 */	Key::T,
/*  85 */	Key::U,
/*  86 */	Key::V,
/*  87 */	Key::W,
/*  88 */	Key::X,
/*  89 */	Key::Y,
/*  90 */	Key::Z,
/*  91 */	Key::Unknown,
/*  92 */	Key::Unknown,
/*  93 */	Key::Unknown,
/*  94 */	Key::Unknown,
/*  95 */	Key::Unknown,
/*  96 */	Key::Unknown,
/*  97 */	Key::a,
/*  98 */	Key::b,
/*  99 */	Key::c,
/* 100 */	Key::d,
/* 101 */	Key::e,
/* 102 */	Key::f,
/* 103 */	Key::g,
/* 104 */	Key::h,
/* 105 */	Key::i,
/* 106 */	Key::j,
/* 107 */	Key::k,
/* 108 */	Key::l,
/* 109 */	Key::m,
/* 110 */	Key::n,
/* 111 */	Key::o,
/* 112 */	Key::p,
/* 113 */	Key::q,
/* 114 */	Key::r,
/* 115 */	Key::s,
/* 116 */	Key::t,
/* 117 */	Key::u,
/* 118 */	Key::v,
/* 119 */	Key::w,
/* 120 */	Key::x,
/* 121 */	Key::y,
/* 122 */	Key::z,
/* 123 */	Key::Unknown,
/* 124 */	Key::Unknown,
/* 125 */	Key::Unknown,
/* 126 */	Key::Unknown,
/* 127 */	Key::Unknown,
/* 128 */	Key::Unknown,
/* 129 */	Key::Unknown,
/* 130 */	Key::Unknown,
/* 131 */	Key::Unknown,
/* 132 */	Key::Unknown,
/* 133 */	Key::Unknown,
/* 134 */	Key::Unknown,
/* 135 */	Key::Unknown,
/* 136 */	Key::Unknown,
/* 137 */	Key::Unknown,
/* 138 */	Key::Unknown,
/* 139 */	Key::Unknown,
/* 140 */	Key::Unknown,
/* 141 */	Key::Unknown,
/* 142 */	Key::Unknown,
/* 143 */	Key::Unknown,
/* 144 */	Key::Unknown,
/* 145 */	Key::Unknown,
/* 146 */	Key::Unknown,
/* 147 */	Key::Unknown,
/* 148 */	Key::Unknown,
/* 149 */	Key::Unknown,
/* 150 */	Key::Unknown,
/* 151 */	Key::Unknown,
/* 152 */	Key::Unknown,
/* 153 */	Key::Unknown,
/* 154 */	Key::Unknown,
/* 155 */	Key::Unknown,
/* 156 */	Key::Unknown,
/* 157 */	Key::Unknown,
/* 158 */	Key::Unknown,
/* 159 */	Key::Unknown,
/* 160 */	Key::Unknown,
/* 161 */	Key::Unknown,
/* 162 */	Key::Unknown,
/* 163 */	Key::Unknown,
/* 164 */	Key::Unknown,
/* 165 */	Key::Unknown,
/* 166 */	Key::Unknown,
/* 167 */	Key::Unknown,
/* 168 */	Key::Unknown,
/* 169 */	Key::Unknown,
/* 170 */	Key::Unknown,
/* 171 */	Key::Unknown,
/* 172 */	Key::Unknown,
/* 173 */	Key::Unknown,
/* 174 */	Key::Unknown,
/* 175 */	Key::Unknown,
/* 176 */	Key::Unknown,
/* 177 */	Key::Unknown,
/* 178 */	Key::Unknown,
/* 179 */	Key::Unknown,
/* 180 */	Key::Unknown,
/* 181 */	Key::Unknown,
/* 182 */	Key::Unknown,
/* 183 */	Key::Unknown,
/* 184 */	Key::Unknown,
/* 185 */	Key::Unknown,
/* 186 */	Key::Unknown,
/* 187 */	Key::Unknown,
/* 188 */	Key::Unknown,
/* 189 */	Key::Unknown,
/* 190 */	Key::Unknown,
/* 191 */	Key::Unknown,
/* 192 */	Key::Unknown,
/* 193 */	Key::Unknown,
/* 194 */	Key::Unknown,
/* 195 */	Key::Unknown,
/* 196 */	Key::Unknown,
/* 197 */	Key::Unknown,
/* 198 */	Key::Unknown,
/* 199 */	Key::Unknown,
/* 200 */	Key::Unknown,
/* 201 */	Key::Unknown,
/* 202 */	Key::Unknown,
/* 203 */	Key::Unknown,
/* 204 */	Key::Unknown,
/* 205 */	Key::Unknown,
/* 206 */	Key::Unknown,
/* 207 */	Key::Unknown,
/* 208 */	Key::Unknown,
/* 209 */	Key::Unknown,
/* 210 */	Key::Unknown,
/* 211 */	Key::Unknown,
/* 212 */	Key::Unknown,
/* 213 */	Key::Unknown,
/* 214 */	Key::Unknown,
/* 215 */	Key::Unknown,
/* 216 */	Key::Unknown,
/* 217 */	Key::Unknown,
/* 218 */	Key::Unknown,
/* 219 */	Key::Unknown,
/* 220 */	Key::Unknown,
/* 221 */	Key::Unknown,
/* 222 */	Key::Unknown,
/* 223 */	Key::Unknown,
/* 224 */	Key::Unknown,
/* 225 */	Key::Unknown,
/* 226 */	Key::Unknown,
/* 227 */	Key::Unknown,
/* 228 */	Key::Unknown,
/* 229 */	Key::Unknown,
/* 230 */	Key::Unknown,
/* 231 */	Key::Unknown,
/* 232 */	Key::Unknown,
/* 233 */	Key::Unknown,
/* 234 */	Key::Unknown,
/* 235 */	Key::Unknown,
/* 236 */	Key::Unknown,
/* 237 */	Key::Unknown,
/* 238 */	Key::Unknown,
/* 239 */	Key::Unknown,
/* 240 */	Key::Unknown,
/* 241 */	Key::Unknown,
/* 242 */	Key::Unknown,
/* 243 */	Key::Unknown,
/* 244 */	Key::Unknown,
/* 245 */	Key::Unknown,
/* 246 */	Key::Unknown,
/* 247 */	Key::Unknown,
/* 248 */	Key::Unknown,
/* 249 */	Key::Unknown,
/* 250 */	Key::Unknown,
/* 251 */	Key::Unknown,
/* 252 */	Key::Unknown,
/* 253 */	Key::Unknown,
/* 254 */	Key::Unknown,
/* 255 */	Key::Unknown
};

Key::Code Manager::sSpecialKeymap[] =
{
/*   0 */	Key::Unknown,
/*   1 */	Key::F1,
/*   2 */	Key::F2,
/*   3 */	Key::F3,
/*   4 */	Key::F4,
/*   5 */	Key::F5,
/*   6 */	Key::F6,
/*   7 */	Key::F7,
/*   8 */	Key::F8,
/*   9 */	Key::F9,
/*  10 */	Key::F10,
/*  11 */	Key::F11,
/*  12 */	Key::F12,
/*  13 */	Key::Unknown,
/*  14 */	Key::Unknown,
/*  15 */	Key::Unknown,
/*  16 */	Key::Unknown,
/*  17 */	Key::Unknown,
/*  18 */	Key::Unknown,
/*  19 */	Key::Unknown,
/*  20 */	Key::Unknown,
/*  21 */	Key::Unknown,
/*  22 */	Key::Unknown,
/*  23 */	Key::Unknown,
/*  24 */	Key::Unknown,
/*  25 */	Key::Unknown,
/*  26 */	Key::Unknown,
/*  27 */	Key::Unknown,
/*  28 */	Key::Unknown,
/*  29 */	Key::Unknown,
/*  30 */	Key::Unknown,
/*  31 */	Key::Unknown,
/*  32 */	Key::Unknown,
/*  33 */	Key::Unknown,
/*  34 */	Key::Unknown,
/*  35 */	Key::Unknown,
/*  36 */	Key::Unknown,
/*  37 */	Key::Unknown,
/*  38 */	Key::Unknown,
/*  39 */	Key::Unknown,
/*  40 */	Key::Unknown,
/*  41 */	Key::Unknown,
/*  42 */	Key::Unknown,
/*  43 */	Key::Unknown,
/*  44 */	Key::Unknown,
/*  45 */	Key::Unknown,
/*  46 */	Key::Unknown,
/*  47 */	Key::Unknown,
/*  48 */	Key::Unknown,
/*  49 */	Key::Unknown,
/*  50 */	Key::Unknown,
/*  51 */	Key::Unknown,
/*  52 */	Key::Unknown,
/*  53 */	Key::Unknown,
/*  54 */	Key::Unknown,
/*  55 */	Key::Unknown,
/*  56 */	Key::Unknown,
/*  57 */	Key::Unknown,
/*  58 */	Key::Unknown,
/*  59 */	Key::Unknown,
/*  60 */	Key::Unknown,
/*  61 */	Key::Unknown,
/*  62 */	Key::Unknown,
/*  63 */	Key::Unknown,
/*  64 */	Key::Unknown,
/*  65 */	Key::Unknown,
/*  66 */	Key::Unknown,
/*  67 */	Key::Unknown,
/*  68 */	Key::Unknown,
/*  69 */	Key::Unknown,
/*  70 */	Key::Unknown,
/*  71 */	Key::Unknown,
/*  72 */	Key::Unknown,
/*  73 */	Key::Unknown,
/*  74 */	Key::Unknown,
/*  75 */	Key::Unknown,
/*  76 */	Key::Unknown,
/*  77 */	Key::Unknown,
/*  78 */	Key::Unknown,
/*  79 */	Key::Unknown,
/*  80 */	Key::Unknown,
/*  81 */	Key::Unknown,
/*  82 */	Key::Unknown,
/*  83 */	Key::Unknown,
/*  84 */	Key::Unknown,
/*  85 */	Key::Unknown,
/*  86 */	Key::Unknown,
/*  87 */	Key::Unknown,
/*  88 */	Key::Unknown,
/*  89 */	Key::Unknown,
/*  90 */	Key::Unknown,
/*  91 */	Key::Unknown,
/*  92 */	Key::Unknown,
/*  93 */	Key::Unknown,
/*  94 */	Key::Unknown,
/*  95 */	Key::Unknown,
/*  96 */	Key::Unknown,
/*  97 */	Key::Unknown,
/*  98 */	Key::Unknown,
/*  99 */	Key::Unknown,
/* 100 */	Key::Left,
/* 101 */	Key::Up,
/* 102 */	Key::Right,
/* 103 */	Key::Down,
/* 104 */	Key::PageUp,
/* 105 */	Key::PageDown,
/* 106 */	Key::Home,
/* 107 */	Key::End,
/* 108 */	Key::Insert,
/* 109 */	Key::Unknown,
/* 110 */	Key::Unknown,
/* 111 */	Key::Unknown,
/* 112 */	Key::Unknown,
/* 113 */	Key::Unknown,
/* 114 */	Key::Unknown,
/* 115 */	Key::Unknown,
/* 116 */	Key::Unknown,
/* 117 */	Key::Unknown,
/* 118 */	Key::Unknown,
/* 119 */	Key::Unknown,
/* 120 */	Key::Unknown,
/* 121 */	Key::Unknown,
/* 122 */	Key::Unknown,
/* 123 */	Key::Unknown,
/* 124 */	Key::Unknown,
/* 125 */	Key::Unknown,
/* 126 */	Key::Unknown,
/* 127 */	Key::Unknown,
/* 128 */	Key::Unknown,
/* 129 */	Key::Unknown,
/* 130 */	Key::Unknown,
/* 131 */	Key::Unknown,
/* 132 */	Key::Unknown,
/* 133 */	Key::Unknown,
/* 134 */	Key::Unknown,
/* 135 */	Key::Unknown,
/* 136 */	Key::Unknown,
/* 137 */	Key::Unknown,
/* 138 */	Key::Unknown,
/* 139 */	Key::Unknown,
/* 140 */	Key::Unknown,
/* 141 */	Key::Unknown,
/* 142 */	Key::Unknown,
/* 143 */	Key::Unknown,
/* 144 */	Key::Unknown,
/* 145 */	Key::Unknown,
/* 146 */	Key::Unknown,
/* 147 */	Key::Unknown,
/* 148 */	Key::Unknown,
/* 149 */	Key::Unknown,
/* 150 */	Key::Unknown,
/* 151 */	Key::Unknown,
/* 152 */	Key::Unknown,
/* 153 */	Key::Unknown,
/* 154 */	Key::Unknown,
/* 155 */	Key::Unknown,
/* 156 */	Key::Unknown,
/* 157 */	Key::Unknown,
/* 158 */	Key::Unknown,
/* 159 */	Key::Unknown,
/* 160 */	Key::Unknown,
/* 161 */	Key::Unknown,
/* 162 */	Key::Unknown,
/* 163 */	Key::Unknown,
/* 164 */	Key::Unknown,
/* 165 */	Key::Unknown,
/* 166 */	Key::Unknown,
/* 167 */	Key::Unknown,
/* 168 */	Key::Unknown,
/* 169 */	Key::Unknown,
/* 170 */	Key::Unknown,
/* 171 */	Key::Unknown,
/* 172 */	Key::Unknown,
/* 173 */	Key::Unknown,
/* 174 */	Key::Unknown,
/* 175 */	Key::Unknown,
/* 176 */	Key::Unknown,
/* 177 */	Key::Unknown,
/* 178 */	Key::Unknown,
/* 179 */	Key::Unknown,
/* 180 */	Key::Unknown,
/* 181 */	Key::Unknown,
/* 182 */	Key::Unknown,
/* 183 */	Key::Unknown,
/* 184 */	Key::Unknown,
/* 185 */	Key::Unknown,
/* 186 */	Key::Unknown,
/* 187 */	Key::Unknown,
/* 188 */	Key::Unknown,
/* 189 */	Key::Unknown,
/* 190 */	Key::Unknown,
/* 191 */	Key::Unknown,
/* 192 */	Key::Unknown,
/* 193 */	Key::Unknown,
/* 194 */	Key::Unknown,
/* 195 */	Key::Unknown,
/* 196 */	Key::Unknown,
/* 197 */	Key::Unknown,
/* 198 */	Key::Unknown,
/* 199 */	Key::Unknown,
/* 200 */	Key::Unknown,
/* 201 */	Key::Unknown,
/* 202 */	Key::Unknown,
/* 203 */	Key::Unknown,
/* 204 */	Key::Unknown,
/* 205 */	Key::Unknown,
/* 206 */	Key::Unknown,
/* 207 */	Key::Unknown,
/* 208 */	Key::Unknown,
/* 209 */	Key::Unknown,
/* 210 */	Key::Unknown,
/* 211 */	Key::Unknown,
/* 212 */	Key::Unknown,
/* 213 */	Key::Unknown,
/* 214 */	Key::Unknown,
/* 215 */	Key::Unknown,
/* 216 */	Key::Unknown,
/* 217 */	Key::Unknown,
/* 218 */	Key::Unknown,
/* 219 */	Key::Unknown,
/* 220 */	Key::Unknown,
/* 221 */	Key::Unknown,
/* 222 */	Key::Unknown,
/* 223 */	Key::Unknown,
/* 224 */	Key::Unknown,
/* 225 */	Key::Unknown,
/* 226 */	Key::Unknown,
/* 227 */	Key::Unknown,
/* 228 */	Key::Unknown,
/* 229 */	Key::Unknown,
/* 230 */	Key::Unknown,
/* 231 */	Key::Unknown,
/* 232 */	Key::Unknown,
/* 233 */	Key::Unknown,
/* 234 */	Key::Unknown,
/* 235 */	Key::Unknown,
/* 236 */	Key::Unknown,
/* 237 */	Key::Unknown,
/* 238 */	Key::Unknown,
/* 239 */	Key::Unknown,
/* 240 */	Key::Unknown,
/* 241 */	Key::Unknown,
/* 242 */	Key::Unknown,
/* 243 */	Key::Unknown,
/* 244 */	Key::Unknown,
/* 245 */	Key::Unknown,
/* 246 */	Key::Unknown,
/* 247 */	Key::Unknown,
/* 248 */	Key::Unknown,
/* 249 */	Key::Unknown,
/* 250 */	Key::Unknown,
/* 251 */	Key::Unknown,
/* 252 */	Key::Unknown,
/* 253 */	Key::Unknown,
/* 254 */	Key::Unknown,
/* 255 */	Key::Unknown
};

#endif

}
