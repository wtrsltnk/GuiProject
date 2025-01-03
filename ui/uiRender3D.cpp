#include "uiControls.h"

#include <glad/glad.h>
#include <glm/ext/scalar_constants.hpp>

// Replaces gluPerspective. Sets the frustum to perspective mode.
// fovY     - Field of vision in degrees in the y direction
// aspect   - Aspect ratio of the viewport
// zNear    - The near clipping distance
// zFar     - The far clipping distance

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble fW = 0, fH = 0;

    // fH = tan((fovY / 2) / 180 * glm::pi<double>()) * zNear;
    fH = tan(fovY / 360 * glm::pi<double>()) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

namespace ui
{

    Render3D::Render3D(int x, int y, int w, int h)
        : Control(ControlTypes::Render3D, x, y, w, h), onRender(Render3DEvent(this))
    {
    }

    Render3D::~Render3D()
    {
    }

    void Render3D::render()
    {
        this->renderBox(true);
        float aspect = 1.0f * ((float)this->width() / (float)this->height());

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        perspectiveGL(45.0f, aspect, 0.1f, 3000.0f);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        event::EventArgs e;
        // this->onRender(&e);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

} // namespace ui
