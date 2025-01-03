#include "uiClipper.h"
#include "uiControls.h"
#include "uiFont.h"

#include <glad/glad.h>

namespace ui
{

    HorizontalContainer::HorizontalContainer(int x, int y, int w, int h)
        : Container(ControlTypes::HorizontalContainer, x, y, w, h)
    {
    }

    HorizontalContainer::~HorizontalContainer()
    {
    }

    void HorizontalContainer::render()
    {
        this->renderBox(false);

        //	this->scrollbar.renderScrollbar();

        float hitbox[4] = {
            this->mBox.hitbox[0] + 3,
            this->mBox.hitbox[1] + 3,
            this->mBox.hitbox[2] - 6,
            this->mBox.hitbox[3] - 6};
        Clipper c(hitbox);

        glPushMatrix();
        //	glTranslatef(0, this->scrollbar.scroll(), 0);
        for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
            (*itr)->renderControl();
        glPopMatrix();
    }

    float HorizontalContainer::clientHeight()
    {
        float childWidth = this->padding();
        for (ControlList::iterator itr = this->controls().begin(); itr != this->controls().end(); ++itr)
            childWidth += (*itr)->width() + this->padding();

        return childWidth;
    }

    void HorizontalContainer::updateChildControls()
    {
        float x = this->mBox.hitbox[0];
        float y = this->mBox.hitbox[1] + this->mBox.hitbox[3];

        for (ControlList::iterator itr = this->mControls.begin(); itr != this->mControls.end(); ++itr)
        {
            Control *c = *itr;
            c->box().hitbox[0] = x + this->mPadding;
            c->box().hitbox[1] = y - this->mPadding - c->height();
            c->box().hitbox[3] = this->height() - (this->mPadding * 2) - this->scrollbar.width();
            c->updateBox();
            x += c->width() + this->mPadding;
            Container *cc = dynamic_cast<Container *>(c);
            if (cc != 0)
                cc->updateChildControls();
        }
    }

} // namespace ui
