/*
 * uiControls.h
 *
 *  Created on: Mar 14, 2011
 *      Author: wouter
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "uiManager.h"
#include "uiUserInput.h"

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
	VerticalContainer = 1,
	FreeContainer = 2,
	Label = 101,
	Button = 102,
	Checkbox = 103,
	Textbox = 104,
	Valuebox = 105,
	Listbox = 106,
	Render3D = 107,
};
}

namespace EventType
{
enum
{
	Click = 0,
	StateChanged,
	TextChanged,
	ValueChanged,
};
}

class Container;

class KeyboardEventArgs : public event::EventArgs
{
public:
	KeyboardEventArgs();
	virtual ~KeyboardEventArgs();

	KeyboardState state;
};

typedef event::Event<Control, KeyboardEventArgs> KeyboardEvent;
typedef KeyboardEvent::Handler KeyboardEventHandler;

class MouseButtonEventArgs : public event::EventArgs
{
public:
	MouseButtonEventArgs();
	virtual ~MouseButtonEventArgs();

	MouseState state;

};

typedef event::Event<Control, MouseButtonEventArgs> MouseEvent;
typedef MouseEvent::Handler MouseButtonEventHandler;

class Control
{
public:
	Control(int type);
	Control(int type, int x, int y, int w, int h);
	virtual ~Control();

	virtual void renderControl();
	virtual void mouseIn();
	virtual void mouseDown(Mouse::Button button);
	virtual void mouseUp(Mouse::Button button);
	virtual void mouseOut();
	virtual void keyDown(Key::Code key);
	virtual void keyUp(Key::Code key);

	box_t& box() { return this->mBox; }
	Container* parent() { return this->mParent; }
	int controlType() { return this->mType; }

	virtual float x();
	virtual float y();
	virtual void setPosition(float pos[2]);
	virtual void setPosition(float x, float y);

	virtual float width();
	virtual float height();
	virtual float clientHeight();
	virtual void setSize(float size[2]);
	virtual void setSize(float w, float h);

	virtual void updateBox();

	virtual bool isPointInBox(float point[2], float scroll = 0);

	KeyboardEvent onKeyboardDown;
	KeyboardEvent onKeyboardUp;
	MouseEvent onMouseButtonDown;
	MouseEvent onMouseButtonUp;
	MouseEvent onMouseMove;
	
protected:
	box_t mBox;
	Container* mParent;

	virtual void render() = 0;

	void renderBox(bool ignoreState = false);
	void renderText(float x, float y, const char *text, unsigned int color);

private:
	int mType;

	friend class Container;
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

class Scrollbar
{
public:
	Scrollbar(Control* container);
	virtual ~Scrollbar();

	void scrollUp();
	void scrollDown();
	float scroll();
	float globalScroll();
	float width();

	virtual void renderScrollbar();

private:
	Control* mControl;
	float mScroll;
	float mWidth;

};

typedef std::vector<Control*> ControlList;

class Container : public Control
{
public:
	Container(int type, int x, int y, int w, int h);
	virtual ~Container();

	virtual void mouseIn() { }
	virtual void mouseDown(Mouse::Button button);
	virtual void mouseUp(Mouse::Button button) { }
	virtual void mouseOut() { }

	virtual float clientHeight();

	void addControl(Control* ctr);
	void removeControl(Control* ctr);

	virtual void setSize(float w, float h);

	float padding();
	void setPadding(float padding);

	ControlList& controls() { return this->mControls; }

	Scrollbar scrollbar;

	virtual void updateChildControls() = 0;
	
protected:
	ControlList mControls;
	float mPadding;

	friend class Control;
};

class VerticalContainer : public Container
{
public:
	VerticalContainer(int x, int y, int w, int h);
	virtual ~VerticalContainer();

	virtual void render();

private:

	virtual void updateChildControls();
};

class FreeContainer : public Container
{
public:
	FreeContainer(int x, int y, int w, int h);
	virtual ~FreeContainer();

	virtual void render();

private:

	virtual void updateChildControls();
};

class SelectedIndexChangedEventArgs : public event::EventArgs
{
public:
	SelectedIndexChangedEventArgs(int previousIndex, int newIndex);
	virtual ~SelectedIndexChangedEventArgs();

	int previousIndex() const;
	int newIndex() const;

private:
	int mPreviousIndex;
	int mNewIndex;
};

typedef event::Event<Control, SelectedIndexChangedEventArgs> SelectedIndexChangedEvent;
typedef SelectedIndexChangedEvent::Handler SelectedIndexChangedEventHandler;

class Listbox : public Control
{
public:
	class ListboxItem
	{
		ListboxItem(const char* text, void* data);
	public:
		
		const char* text() { return this->mText; }
		void* data() { return this->mData; }

	private:
		const char* mText;
		void* mData;

		friend class Listbox;
	};

public:
	Listbox(int x, int y, int w, int h);
	virtual ~Listbox();

	virtual void render();
	virtual void mouseDown(Mouse::Button button);

	virtual float clientHeight();

	void addItem(const char* text, void* data = 0);
	int selectedIndex() const;
	void setSelectedIndex(int index);
	ListboxItem selectedItem() const;

	Scrollbar scrollbar;
	SelectedIndexChangedEvent SelectedIndexChanged;
	
private:
	std::vector<ListboxItem> mItems;
	float mPadding;
	int mSelectedIndex;

};

typedef event::Event<Control, event::EventArgs> ClickEvent;
typedef ClickEvent::Handler ClickEventHandler;

class Button : public Label
{
public:
	Button(int x, int y, int w, int h, const char* text);
	virtual ~Button();

	virtual void render();
	virtual void mouseDown(Mouse::Button button);

	ClickEvent Click;

};

typedef event::Event<Control, event::EventArgs> StateChangedEvent;
typedef StateChangedEvent::Handler StateChangedEventHandler;

class Checkbox : public Label
{
public:
	Checkbox(int x, int y, int w, int h, const char* text);
	virtual ~Checkbox();

	virtual void render();
	virtual void mouseDown(Mouse::Button button);

	bool checked();
	void setChecked(bool state);
	void toggleChecked();

	StateChangedEvent StateChanged;

	virtual void updateBox();

private:
	bool mChecked;

};

typedef event::Event<Control, event::EventArgs> TextChangedEvent;
typedef TextChangedEvent::Handler TextChangedEventHandler;

class Textbox : public Label
{
public:
	Textbox(int x, int y, int w, int h, const char* text);
	virtual ~Textbox();

	virtual void render();
	virtual void keyDown(Key::Code key);

	void setText(const char* text);
	void addChar(char c);
	char removeChar();
	void moveCursor(int amount);

	TextChangedEvent TextChanged;

	virtual void updateBox();

private:
	int mTextLength;
	int mBufferLength;
	int mCursorIndex;
	int mCursorPosition;
	float mScroll;
	int mPadding;

	void setCursorIndex(int index);
};

class ValueChangedEventArgs : public event::EventArgs
{
public:
	ValueChangedEventArgs(float originalValue, float newValue) : mOriginalValue(originalValue), mNewValue(newValue) { }
	virtual ~ValueChangedEventArgs() { }

	float originalValue() {return this->mOriginalValue; }
	float newValue() { return this->mNewValue; }

private:
	float mOriginalValue;
	float mNewValue;

};

typedef event::Event<Control, ValueChangedEventArgs> ValueChangedEvent;
typedef ValueChangedEvent::Handler ValueChangedEventHandler;

class Valuebox : public Control
{
public:
	Valuebox(int x, int y, int w, int h, float value, float min, float max);
	virtual ~Valuebox();

	virtual void render();
	virtual void keyDown(Key::Code key);

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

typedef event::Event<Control, event::EventArgs> Render3DEvent;
typedef Render3DEvent::Handler Render3DEventHandler;

class Render3D : public Control
{
public:
	Render3D(int x, int y, int w, int h);
	virtual ~Render3D();
	
	virtual void render();

	Render3DEvent onRender;
};

}	/* namespace ui */

#endif	/* CONTROLS_H */
