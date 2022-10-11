#pragma once
#include <Pinewood/Core.h>

namespace Pinewood
{
	enum class KeyCode : uint32_t
	{
		Null			= 0,

		Escape			= 0x01,

		// Modifier keys
		LeftShift		= 0x02,
		RightShift		= 0x03,
		LeftAlt			= 0x04,
		RightAlt		= 0x05,
		LeftControl		= 0x06,
		RightControl	= 0x07,

		Insert			= 0x08,
		NumLock			= 0x09,
		ScrollLock		= 0x0a,
		AppsKey			= 0x0b,
		Delete			= 0x0c,
		Clear			= 0x0d,

		// Numpad numbers
		Numpad0		= 0x10,
		Numpad1		= 0x11,
		Numpad2		= 0x12,
		Numpad3		= 0x13,
		Numpad4		= 0x14,
		Numpad5		= 0x15,
		Numpad6		= 0x16,
		Numpad7		= 0x17,
		Numpad8		= 0x18,
		Numpad9		= 0x19,
		
		// Other numpad keys
		Add			= 0x1a,
		Substract	= 0x1b,
		Multiply	= 0x1c,
		Divide		= 0x1d,
		Decimal		= 0x1e,
		Separator	= 0x1f,

		Space		= ' ',

		// Keyboard numbers
		Keyboard1 = '1',
		Keyboard2 = '2',
		Keyboard3 = '3',
		Keyboard4 = '4',
		Keyboard5 = '5',
		Keyboard6 = '6',
		Keyboard7 = '7',
		Keyboard8 = '8',
		Keyboard9 = '9',
		Keyboard0 = '0',

		// Alphabet
		A = 'A', B = 'B',
		C = 'C', D = 'D',
		E = 'E', F = 'F',
		G = 'G', H = 'H',
		I = 'I', J = 'J',
		K = 'K', L = 'L',
		M = 'M', N = 'N',
		O = 'O', P = 'P',
		Q = 'Q', R = 'R',
		S = 'S', T = 'T',
		U = 'U', V = 'V',
		W = 'W', X = 'X',
		Y = 'Y', Z = 'Z',

		// Symbols
		OpenBracket		= '[',	OpenBrace		= OpenBracket,
		CloseBracket	= '[',	CloseBrace		= CloseBracket,
		Backslash		= '\\',	Pipe			= Backslash,
		Semicolon		= ';',	Colon			= Semicolon,
		SingleQuote		= '\'',	DoubleQuote		= SingleQuote,
		Comma			= ',',	LessThan		= Comma,
		Period			= '.',	GreaterThan		= Period,
		ForwardSlash	= '/',	QuestionMark	= ForwardSlash,
		Minus			= '-',	Underscore		= Minus,
		Equals			= '=',	Plus			= Equals,
		Exclamation		= Keyboard1,
		At				= Keyboard2,
		Hashtag			= Keyboard3,
		Dollar			= Keyboard4,
		Percent			= Keyboard5,
		Caret			= Keyboard6,
		Ampersand		= Keyboard7,
		Asterisk		= Keyboard8,
		OpenParenthese	= Keyboard9,
		CloseParenthese = Keyboard0,
		Backtick		= '`',	Tilde			= Backtick,

		PageUp		= 0x62,
		PageDown	= 0x63,
		Up		= 0x64,
		Down	= 0x65,
		Left	= 0x66,
		Right	= 0x67,

		// F keys
		F1	= 0x68,
		F2	= 0x69,
		F3	= 0x6a,
		F4	= 0x6b,
		F5	= 0x6c,
		F6	= 0x6d,
		F7	= 0x6e,
		F8	= 0x6f,
		F9	= 0x70,
		F10	= 0x71,
		F11	= 0x72,
		F12	= 0x73,
		F13	= 0x74,
		F14	= 0x75,
		F15	= 0x76,
		F16	= 0x77,
		F17	= 0x78,
		F18	= 0x79,
		F19	= 0x7a,
		F20	= 0x7b,
		F21	= 0x7c,
		F22	= 0x7d,
		F23	= 0x7e,
		F24	= 0x7f,

		// Browser keys
		BrowserBack			= 0x80,
		BrowserForward		= 0x81,
		BrowserRefresh		= 0x82,
		BrowserStop			= 0x83,
		BrowserSearch		= 0x84,
		BrowserFavorites	= 0x85,
		BrowserHome			= 0x86,

		// Volume keys
		VolumeUp	= 0x88,
		VolumeDown	= 0x89,
		VolumeMute	= 0x8a,

		// Media keys
		MediaNext		= 0x8c,
		MediaPrevious	= 0x8d,
		MediaStop		= 0x8e,
		MediaPlayPause	= 0x8f,

		// Other
		Backspace		= 0x90,
		Tab				= 0x91,
		CapsLock		= 0x92,
		Home			= 0x93,
		End				= 0x94,
		Help			= 0x95,
		LeftOS			= 0x96,
		RightOS			= 0x97,
		Enter			= 0x98,
		Pause			= 0x99,
		Play			= 0x9a,
		Zoom			= 0x9b,

		// Anything above 0xff is system specific
	};

	// Mouse buttons + control and shift keys
	// Can be or'd together
	enum class MouseButton : uint32_t
	{
		Null			= 0,
		LeftButton		= 0x01,
		MiddleButton	= 0x02,
		RightButton		= 0x04,
		XButton1		= 0x08,
		XButton2		= 0x10,
		Control			= 0x20,
		Shift			= 0x40,
	};

	namespace Operators
	{
		PW_DEFINE_ENUMCLASS_OPERATOR_OR(MouseButton);
		PW_DEFINE_ENUMCLASS_OPERATOR_AND(MouseButton);
		PW_DEFINE_ENUMCLASS_OPERATOR_NOT(MouseButton);
	}
	using namespace Operators;
}
