/*
 * Controls.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "GuiManager.h"

namespace ui
{

namespace BoxState
{
enum
{
	None = 0x00,
	Hovered = 0x01,
	Pressed = 0x02
};
}

namespace VerticalAlignment
{
enum
{
	Top = 1,
	Middle = -2,
	Bottom = -1
};
}

namespace HorizontalAlignment
{
enum
{
	Left = 1,
	Center = -2,
	Right = -1
};
}

struct box_t
{
	Font* font;
	float boxPosition[2];
	float boxSize[2];
	float hitbox[4];
	int state;
	int aligmentHorizontal;
	int aligmentVertical;

	bool isPointInBox(float point[2]);
};

namespace ControlTypes
{
enum
{
	Label = 0,
	Button = 1,
	Checkbox = 2,
	Textbox = 3,
	Valuebox = 4
};
}

class Control
{
public:
	Control(int type);
	Control(int type, int x, int y, int w, int h);
	virtual ~Control();

	void renderControl();

	int getType() { return this->mType; }

	virtual void position(float pos[2]);
	virtual void setPosition(float pos[2]);
	virtual void setPosition(float x, float y);

	virtual void size(float size[2]);
	virtual void setSize(float size[2]);
	virtual void setSize(float w, float h);

	virtual void updateBox();

protected:
	box_t box;

	virtual void render() = 0;

	void renderBox(bool ignoreState = false);
	void renderText(float x, float y, const char *text, unsigned int color);
private:
	int mType;
	friend class GuiManager;
};

class Label : public Control
{
public:
	Label(const char* text, int type = ControlTypes::Label);
	Label(const char* text, int x, int y, int w, int h, int type = ControlTypes::Label);
	virtual ~Label();

	virtual void render();

	virtual const char* text() const;
	virtual void setText(const char* text);

protected:
	char* mText;

};

typedef Event<EventType::Click> ClickEvent;
typedef ClickEvent::Handler ClickEventHandler;

class Button : public Label
{
public:
	Button(int x, int y, int w, int h, const char* text);
	virtual ~Button();

	virtual void render();

	ClickEvent Click;

};

typedef Event<EventType::StateChanged> StateChangedEvent;
typedef StateChangedEvent::Handler StateChangedEventHandler;

class Checkbox : public Label
{
public:
	Checkbox(int x, int y, int w, int h, const char* text);
	virtual ~Checkbox();

	virtual void render();

	bool checked();
	void setChecked(bool state);
	void toggleChecked();

	StateChangedEvent StateChanged;

	virtual void updateBox();

private:
	bool mChecked;

};

class Textbox : public Label
{
public:
	Textbox(int x, int y, int w, int h, const char* text);
	virtual ~Textbox();

	virtual void render();

	void setText(const char* text);
	void addChar(char c);
	char removeChar();
	void moveCursor(int amount);

private:
	int mTextLength;
	int mBufferLength;
	int mCursorIndex;
	int mCursorPosition;

};

class Valuebox : public Control
{
public:
	Valuebox(int x, int y, int w, int h, float value, float min, float max);
	virtual ~Valuebox();

	virtual void render();

	float value();
	void setValue(float value);
	float minValue();
	void setMinValue(float min);
	float maxValue();
	void setMaxValue(float max);

	void addInput(char key);

protected:
	float mValue;
	float mMinValue;
	float mMaxValue;
	char mInput[32];

};

}	/* namespace ui */

#endif	/* CONTROLS_H */
