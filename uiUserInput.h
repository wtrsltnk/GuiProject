/*
 * File:   userinput.h
 * Author: wouter
 *
 * Created on June 12, 2010, 4:45 PM
 */

#ifndef _USERINPUT_H
#define	_USERINPUT_H

/// Definitions for the keys on a keyboard
namespace Key
{
    enum Code
    {
        A = 'a',
        B = 'b',
        C = 'c',
        D = 'd',
        E = 'e',
        F = 'f',
        G = 'g',
        H = 'h',
        I = 'i',
        J = 'j',
        K = 'k',
        L = 'l',
        M = 'm',
        N = 'n',
        O = 'o',
        P = 'p',
        Q = 'q',
        R = 'r',
        S = 's',
        T = 't',
        U = 'u',
        V = 'v',
        W = 'w',
        X = 'x',
        Y = 'y',
        Z = 'z',
        Num0 = '0',
        Num1 = '1',
        Num2 = '2',
        Num3 = '3',
        Num4 = '4',
        Num5 = '5',
        Num6 = '6',
        Num7 = '7',
        Num8 = '8',
        Num9 = '9',
        Escape = 256,
        LControl,
        LShift,
        LAlt,
        LSystem,      ///< OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
        RControl,
        RShift,
        RAlt,
        RSystem,      ///< OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
        Menu,
		Capslock,
		Scrollock,
		Numlock,
        LBracket,     ///< [
        RBracket,     ///< ]
        SemiColon,    ///< ;
        Comma,        ///< ,
        Period,       ///< .
        Quote,        ///< '
        Slash,        ///< /
        BackSlash,
        Tilde,        ///< ~
        Equal,        ///< =
        Dash,         ///< -
        Space,
        Return,
        Back,
        Tab,
        PageUp,
        PageDown,
        End,
        Home,
        Insert,
        Delete,
        Add,          ///< +
        Subtract,     ///< -
        Multiply,     ///< *
        Divide,       ///< /
        Left,         ///< Left arrow
        Right,        ///< Right arrow
        Up,           ///< Up arrow
        Down,         ///< Down arrow
        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        Pause,

		Unknown,
        Count // For internal use
    };
}

class KeyboardState
{
	KeyboardState();
	static KeyboardState sCurrentState;
public:
	static const KeyboardState& currentState();
	KeyboardState(const KeyboardState& state);
	virtual ~KeyboardState();

	bool isKeyPressed(Key::Code key);

private:
	bool mKeys[Key::Count];

//	friend class OpenGLImpl;
};

/// Definitions for the Mouse buttons
namespace Mouse
{
    enum Button
    {
        Left,
        Right,
        Middle,
        XButton1,
        XButton2,

		Unknown,
        Count // For internal use
    };
}

class MouseState
{
	MouseState();
	static MouseState sCurrentState;
public:
	static const MouseState& currentState();
	MouseState(const MouseState& state);
	virtual ~MouseState();

	bool isButtonPressed(Mouse::Button button) const;
	int getMousePositionX() const;
	int getMousePositionY() const;

private:
	bool mMouseButton[Mouse::Count];
	int mMouseX, mMouseY;

//	friend class OpenGLImpl;
};

#endif	/* _USERINPUT_H */
