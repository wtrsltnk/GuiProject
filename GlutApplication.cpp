#include "GlutApplication.h"
#include "ui/uiManager.h"
#include <stdlib.h>

void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void specialKeyboardUp(int key, int x, int y);
void mouseClick(int button, int state, int x, int y);
void mouseMove(int x, int y);
void display();

bool running = true;

void idle(int value)
{
	glutTimerFunc(30, &idle, 0);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);

	glutCreateWindow(gApplication->title);
	glutKeyboardFunc(&keyboard);
	glutKeyboardUpFunc(&keyboardUp);
	glutSpecialFunc(&specialKeyboard);
	glutSpecialUpFunc(&specialKeyboardUp);
	glutMouseFunc(&mouseClick);
	glutMotionFunc(&mouseMove);
	glutPassiveMotionFunc(&mouseMove);
	glutReshapeFunc(&resize);
	glutDisplayFunc(&display);
	glutTimerFunc(30, &idle, 0);

	if (gApplication != 0)
	{
		if (gApplication->initialize(argc, argv) != false)
		{
			glutMainLoop();
		}
	}

	delete gApplication;
	gApplication = 0;

	return EXIT_SUCCESS;
}

void display()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gApplication != 0)
	{
		gApplication->render(time);
	}

	glutSwapBuffers();
}

void resize(int w, int h)
{
	if (gApplication != 0)
	{
		gApplication->width = w;
		if (gApplication->width <= 0) gApplication->width = 1;
		gApplication->height = h;
		if (gApplication->height <= 0) gApplication->height = 1;
	}

	if (gApplication != 0)
		gApplication->resize(gApplication->width, gApplication->height);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '\x1B':
		{
			glutLeaveMainLoop();
			break;
		}
	default:
		{
			if (gApplication != 0)
				gApplication->onKeyboard(key, x, y);
		}
	}
	ui::Manager::glutKeyboardDown(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y)
{
	if (gApplication != 0)
		gApplication->onKeyboardUp(key, x, y);
	ui::Manager::glutKeyboardUp(key, x, y);
}

void specialKeyboard(int key, int x, int y)
{
	if (gApplication != 0)
		gApplication->onSpecialKeyboard(key, x, y);
	ui::Manager::glutSpecialKeyboardDown(key, x, y);
}

void specialKeyboardUp(int key, int x, int y)
{
	if (gApplication != 0)
		gApplication->onSpecialKeyboardUp(key, x, y);
	ui::Manager::glutSpecialKeyboardUp(key, x, y);
}

void mouseClick(int button, int state, int x, int y)
{
	if (gApplication != 0)
		gApplication->onMouseClick(button, state, x, y);
	glutPostRedisplay();
	ui::Manager::glutMouseClick(button, state, x, y);
}

void mouseMove(int x, int y)
{
	if (gApplication != 0)
		gApplication->onMouseMove(x, y);
	glutPostRedisplay();
	ui::Manager::glutMouseMove(x, y);
}
