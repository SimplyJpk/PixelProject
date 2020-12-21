#pragma once
#include "SDL.h"

// Keys https://wiki.libsdl.org/SDLScancodeLookup
// Max value from https://wiki.libsdl.org/SDLScancodeLookup
#define SCANCODE_MAXSIZE 284

//TODO Double check, do we need any other inputs? This should cover most.
enum class KeyCode
{
		UNKNOWN = SDL_SCANCODE_UNKNOWN,
		// Letters
		A = SDL_SCANCODE_A,
		B = SDL_SCANCODE_B,
		C = SDL_SCANCODE_C,
		D = SDL_SCANCODE_D,
		E = SDL_SCANCODE_E,
		F = SDL_SCANCODE_F,
		G = SDL_SCANCODE_G,
		H = SDL_SCANCODE_H,
		I = SDL_SCANCODE_I,
		J = SDL_SCANCODE_J,
		K = SDL_SCANCODE_K,
		L = SDL_SCANCODE_L,
		M = SDL_SCANCODE_M,
		N = SDL_SCANCODE_N,
		O = SDL_SCANCODE_O,
		P = SDL_SCANCODE_P,
		Q = SDL_SCANCODE_Q,
		R = SDL_SCANCODE_R,
		S = SDL_SCANCODE_S,
		T = SDL_SCANCODE_T,
		U = SDL_SCANCODE_U,
		V = SDL_SCANCODE_V,
		W = SDL_SCANCODE_W,
		X = SDL_SCANCODE_X,
		Y = SDL_SCANCODE_Y,
		Z = SDL_SCANCODE_Z,
		// Numbers
		_0 = SDL_SCANCODE_0,
		_1 = SDL_SCANCODE_1,
		_2 = SDL_SCANCODE_2,
		_3 = SDL_SCANCODE_3,
		_4 = SDL_SCANCODE_4,
		_5 = SDL_SCANCODE_5,
		_6 = SDL_SCANCODE_6,
		_7 = SDL_SCANCODE_7,
		_8 = SDL_SCANCODE_8,
		_9 = SDL_SCANCODE_9,
		// Format
		Return = SDL_SCANCODE_RETURN,
		Escape = SDL_SCANCODE_ESCAPE,
		Backspace = SDL_SCANCODE_BACKSPACE,
		Tab = SDL_SCANCODE_TAB,
		Space = SDL_SCANCODE_SPACE,
		Minus = SDL_SCANCODE_MINUS,
		Equals = SDL_SCANCODE_EQUALS,
		LeftBracket = SDL_SCANCODE_LEFTBRACKET,
		RightBracket = SDL_SCANCODE_RIGHTBRACKET,
		Backslash = SDL_SCANCODE_BACKSLASH,
		//x NonBackslash = SDL_SCANCODE_NONUSHASH, // Do we need this?
		SemiColon = SDL_SCANCODE_SEMICOLON,
		Apostrophe = SDL_SCANCODE_APOSTROPHE,
		//x Grave = SDL_SCANCODE_GRAVE,
		Comma = SDL_SCANCODE_COMMA,
		Period = SDL_SCANCODE_PERIOD,
		Slash = SDL_SCANCODE_SLASH,
		Caps = SDL_SCANCODE_CAPSLOCK,
		// F Keys
		F1 = SDL_SCANCODE_F1,
		F2 = SDL_SCANCODE_F2,
		F3 = SDL_SCANCODE_F3,
		F4 = SDL_SCANCODE_F4,
		F5 = SDL_SCANCODE_F5,
		F6 = SDL_SCANCODE_F6,
		F7 = SDL_SCANCODE_F7,
		F8 = SDL_SCANCODE_F8,
		F9 = SDL_SCANCODE_F9,
		// Directions
		Right = SDL_SCANCODE_RIGHT,
		Left = SDL_SCANCODE_LEFT,
		Down = SDL_SCANCODE_DOWN,
		Up = SDL_SCANCODE_UP,
};

enum MouseCode {
		MouseLeft,
		MouseRight,
		MouseMiddle,
		MouseClickTypeCount
};
