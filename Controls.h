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

};

namespace ControlTypes
{
enum
{
	Container = 1,
	Label = 101,
	Button = 102,
	Checkbox = 103,
	Textbox = 104,
	Valuebox = 105,
	Listbox = 106,
};
}

class Container;

class Control
{
public:
	Control(int type);
	Control(int type, int x, int y, int w, int h);
	virtual ~Control();

	virtual void renderControl();

	int getType() { return this->mType; }

	virtual void position(float pos[2]);
	virtual float x();
	virtual float y();
	virtual void setPosition(float pos[2]);
	virtual void setPosition(float x, float y);

	virtual void size(float size[2]);
	virtual float width();
	virtual float height();
	virtual void setSize(float size[2]);
	virtual void setSize(float w, float h);

	virtual void updateBox();

	bool isPointInBox(float point[2]);
protected:
	box_t mBox;
	Container* mParent;

	virtual void render() = 0;

	void renderBox(bool ignoreState = false);
	void renderText(float x, float y, const char *text, unsigned int color);
private:
	int mType;
	friend class GuiManager;
	friend class Container;
};

typedef std::vector<Control*> ControlList;

class Container : public Control
{
public:
	Container(int x, int y, int w, int h);
	virtual ~Container();

	virtual void render();

	void addControl(Control* ctr);
	void removeControl(Control* ctr);

	float padding();
	void setPadding(float padding);

	void scrollUp();
	void scrollDown();
	float getScroll();

	ControlList& getControls() { return this->mControls; }

private:
	ControlList mControls;
	float mPadding;
	float mChildHeight;
	float mScroll;

	void updateChildControls();

	friend class Control;
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

typedef Event<EventType::TextChanged> TextChangedEvent;
typedef TextChangedEvent::Handler TextChangedEventHandler;

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

	TextChangedEvent TextChanged;

private:
	int mTextLength;
	int mBufferLength;
	int mCursorIndex;
	int mCursorPosition;
	float mScroll;
	int mPadding;

	void setCursorIndex(int index);
};

typedef Event<EventType::ValueChanged> ValueChangedEvent;
typedef ValueChangedEvent::Handler ValueChangedEventHandler;

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

class Listbox : public Control
{
public:
	Listbox(int x, int y, int w, int h);
	virtual ~Listbox();

	virtual void render();

	int addItem(const char* text, void* data);
	void removeItem(int index);

private:
	class ListboxItem
	{
	public:
		ListboxItem(const char* text, void* data) : mText(text), mData(data) { }

		const char* mText;
		void* mData;
	};

	std::vector<ListboxItem> mItems;

};

}	/* namespace ui */

#endif	/* CONTROLS_H */
