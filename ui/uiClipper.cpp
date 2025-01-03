/*
 * File:   Clipper.cpp
 * Author: wouter
 *
 * Created on March 26, 2011, 9:23 AM
 */

#include "uiClipper.h"

#include <glad/glad.h>

int Clipper::stack = 0;

Clipper::Clipper(float hitbox[4])
{
    for (int i = 0; i < 4; i++)
        this->hitbox[i] = hitbox[i];
    glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
    glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
    Clipper::stack++;
    // only draw to stencil buffer
    glColorMask(0, 0, 0, 0);
    glStencilMask(0xFF);
    glBegin(GL_QUADS);
    glVertex2f(this->hitbox[0], this->hitbox[1]);
    glVertex2f(this->hitbox[0] + this->hitbox[2], this->hitbox[1]);
    glVertex2f(this->hitbox[0] + this->hitbox[2], this->hitbox[1] + this->hitbox[3]);
    glVertex2f(this->hitbox[0], this->hitbox[1] + this->hitbox[3]);
    glEnd();

    // Stencil clipper drawn,
    glColorMask(1, 1, 1, 1);
    glStencilMask(0);
    // now only draw stuff that's that has the right clipper value
    glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

Clipper::~Clipper()
{
    // Decrement anything we previously incremented
    glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
    glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
    Clipper::stack--;
    // Only draw to stencil buffer
    glColorMask(0, 0, 0, 0);
    glStencilMask(0xFF);
    glBegin(GL_QUADS);
    glVertex2f(this->hitbox[0], this->hitbox[1]);
    glVertex2f(this->hitbox[0] + this->hitbox[2], this->hitbox[1]);
    glVertex2f(this->hitbox[0] + this->hitbox[2], this->hitbox[1] + this->hitbox[3]);
    glVertex2f(this->hitbox[0], this->hitbox[1] + this->hitbox[3]);
    glEnd();

    // now draw on regular color buffer again,
    // stencil buffer should be the same as before constructor call
    glColorMask(1, 1, 1, 1);
    glStencilMask(0);
    glStencilFunc(GL_EQUAL, Clipper::stack, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}
