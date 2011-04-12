/*
 * File:   userinput.h
 * Author: wouter
 *
 * Created on June 12, 2010, 4:45 PM
 */

#ifndef _USERINPUT_H
#define	_USERINPUT_H

namespace ui
{

/// Definitions for the keys on a keyboard
namespace Key
{
    enum Code
    {
        a = 'a',
        b = 'b',
        c = 'c',
        d = 'd',
        e = 'e',
        f = 'f',
        g = 'g',
        h = 'h',
        i = 'i',
        j = 'j',
        k = 'k',
        l = 'l',
        m = 'm',
        n = 'n',
        o = 'o',
        p = 'p',
        q = 'q',
        r = 'r',
        s = 's',
        t = 't',
        u = 'u',
        v = 'v',
        w = 'w',
        x = 'x',
        y = 'y',
        z = 'z',
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
        I = 'I',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        N = 'N',
        O = 'O',
        P = 'P',
        Q = 'Q',
        R = 'R',
        S = 'S',
        T = 'T',
        U = 'U',
        V = 'V',
        W = 'W',
        X = 'X',
        Y = 'Y',
        Z = 'Z',
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
        Add,          ///< +
        Subtract,     ///< -
        Multiply,     ///< *
        Divide,       ///< /
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

	bool isKeyPressed(Key::Code key) const;

private:
	bool mKeys[Key::Count];

	friend class Manager;
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

	friend class Manager;
};

}

#endif	/* _USERINPUT_H */
