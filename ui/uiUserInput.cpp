#include "uiUserInput.h"

namespace ui
{

/**************************************************************/
/** Mouse State												 **/
/**************************************************************/

MouseState MouseState::sCurrentState;

MouseState::MouseState()
{
	for (int i = 0; i < Mouse::Count; i++)
		this->mMouseButton[i] = false;
	this->mMouseX = this->mMouseY = 0;
}

MouseState::MouseState(const MouseState& state)
{
	for (int i = 0; i < Mouse::Count; i++)
		this->mMouseButton[i] = state.mMouseButton[i];
	this->mMouseX = state.mMouseX;
	this->mMouseY = state.mMouseY;
}

const MouseState& MouseState::currentState()
{
	return MouseState::sCurrentState;
}

MouseState::~MouseState()
{
}

bool MouseState::isButtonPressed(Mouse::Button button) const
{
	return this->mMouseButton[button];
}

int MouseState::getMousePositionX() const
{
	return this->mMouseX;
}

int MouseState::getMousePositionY() const
{
	return this->mMouseY;
}


/**************************************************************/
/** Keyboard State											 **/
/**************************************************************/

KeyboardState KeyboardState::sCurrentState;

KeyboardState::KeyboardState()
{
	for (int i = 0; i < Key::Count; i++)
		this->mKeys[i] = false;
}

const KeyboardState& KeyboardState::currentState()
{
	return KeyboardState::sCurrentState;
}

KeyboardState::KeyboardState(const KeyboardState& state)
{
	for (int i = 0; i < Key::Count; i++)
		this->mKeys[i] = state.mKeys[i];
}

KeyboardState::~KeyboardState()
{
}

bool KeyboardState::isKeyPressed(Key::Code key) const
{
	return this->mKeys[key];
}

}
