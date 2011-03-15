#include "GlutApplication.h"
#include <stdlib.h>

void resize(int w, int h);
void display();

bool running = true;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	glutCreateWindow(gApplication->title);
	glutReshapeFunc(&resize);
	glutDisplayFunc(&display);
	if (gApplication != 0)
	{
		if (gApplication->initialize() != false)
		{
			while(running)
			{
				glutMainLoopEvent();
			}
		}
	}

	delete gApplication;
	gApplication = 0;

	return EXIT_SUCCESS;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gApplication != 0)
	{
		gApplication->render();
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
