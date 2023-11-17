#pragma once

#include <Windows.h>

enum Key {
	W, A, S, D, Q, E, LEFT_SHIFT,
	UP, DOWN, LEFT, RIGHT, COMMA, PERIOD, SLASH,
	COUNT
};


class Input {
	struct KeyboardInput {
		bool is_down = false;
	};

	static KeyboardInput keys[];

public:
	static KeyboardInput GetKey(Key key);
	static bool GetKeyDown(Key key);
	static void HandleKeyboardInput(UINT_PTR keyCode, bool isDown);
	static void Reset();
};
