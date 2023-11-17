#include "Input.h"

Input::KeyboardInput Input::keys[Key::COUNT];

Input::KeyboardInput Input::GetKey(Key key) {
	return keys[key];
}

bool Input::GetKeyDown(Key key) {
	return keys[key].is_down;
}

void Input::HandleKeyboardInput(UINT_PTR key_code, bool is_down) {
	switch (key_code) {
	case 'W':
		keys[W].is_down = is_down;
		break;
	case 'A':
		keys[A].is_down = is_down;
		break;
	case 'S':
		keys[S].is_down = is_down;
		break;
	case 'D':
		keys[D].is_down = is_down;
		break;
	case 'Q':
		keys[Q].is_down = is_down;
		break;
	case 'E':
		keys[E].is_down = is_down;
		break;
	case VK_SHIFT:
		keys[LEFT_SHIFT].is_down = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
		break;
	case VK_UP:
		keys[UP].is_down = is_down;
		break;
	case VK_DOWN:
		keys[DOWN].is_down = is_down;
		break;
	case VK_LEFT:
		keys[LEFT].is_down = is_down;
		break;
	case VK_RIGHT:
		keys[RIGHT].is_down = is_down;
		break;
	case VK_OEM_COMMA:
		keys[COMMA].is_down = is_down;
		break;
	case VK_OEM_PERIOD:
		keys[PERIOD].is_down = is_down;
		break;
	case VK_OEM_2:
		keys[SLASH].is_down = is_down;
		break;
	}
}

void Input::Reset() {
	for (int i = 0; i < Key::COUNT; i++) {
		keys[i].is_down = false;
	}
}
