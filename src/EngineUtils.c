#include "EngineUtils.h"

uint     width        = 800;
uint     height       = 480;
ulong    frameCount   = 0;
Camera2D globalCamera = {.offset = {.x = 0, .y = 0}, .rotation = 0, .target = {.x = 0, .y = 0}, .zoom = 1};

float KeyboardKeyPressTimers[] = {
	[KEY_NULL]          = 0,
	[KEY_APOSTROPHE]    = 0,
	[KEY_COMMA]         = 0,
	[KEY_MINUS]         = 0,
	[KEY_PERIOD]        = 0,
	[KEY_SLASH]         = 0,
	[KEY_ZERO]          = 0,
	[KEY_ONE]           = 0,
	[KEY_TWO]           = 0,
	[KEY_THREE]         = 0,
	[KEY_FOUR]          = 0,
	[KEY_FIVE]          = 0,
	[KEY_SIX]           = 0,
	[KEY_SEVEN]         = 0,
	[KEY_EIGHT]         = 0,
	[KEY_NINE]          = 0,
	[KEY_SEMICOLON]     = 0,
	[KEY_EQUAL]         = 0,
	[KEY_A]             = 0,
	[KEY_B]             = 0,
	[KEY_C]             = 0,
	[KEY_D]             = 0,
	[KEY_E]             = 0,
	[KEY_F]             = 0,
	[KEY_G]             = 0,
	[KEY_H]             = 0,
	[KEY_I]             = 0,
	[KEY_J]             = 0,
	[KEY_K]             = 0,
	[KEY_L]             = 0,
	[KEY_M]             = 0,
	[KEY_N]             = 0,
	[KEY_O]             = 0,
	[KEY_P]             = 0,
	[KEY_Q]             = 0,
	[KEY_R]             = 0,
	[KEY_S]             = 0,
	[KEY_T]             = 0,
	[KEY_U]             = 0,
	[KEY_V]             = 0,
	[KEY_W]             = 0,
	[KEY_X]             = 0,
	[KEY_Y]             = 0,
	[KEY_Z]             = 0,
	[KEY_LEFT_BRACKET]  = 0,
	[KEY_BACKSLASH]     = 0,
	[KEY_RIGHT_BRACKET] = 0,
	[KEY_GRAVE]         = 0,
	[KEY_SPACE]         = 0,
	[KEY_ESCAPE]        = 0,
	[KEY_ENTER]         = 0,
	[KEY_TAB]           = 0,
	[KEY_BACKSPACE]     = 0,
	[KEY_INSERT]        = 0,
	[KEY_DELETE]        = 0,
	[KEY_RIGHT]         = 0,
	[KEY_LEFT]          = 0,
	[KEY_DOWN]          = 0,
	[KEY_UP]            = 0,
	[KEY_PAGE_UP]       = 0,
	[KEY_PAGE_DOWN]     = 0,
	[KEY_HOME]          = 0,
	[KEY_END]           = 0,
	[KEY_CAPS_LOCK]     = 0,
	[KEY_SCROLL_LOCK]   = 0,
	[KEY_NUM_LOCK]      = 0,
	[KEY_PRINT_SCREEN]  = 0,
	[KEY_PAUSE]         = 0,
	[KEY_F1]            = 0,
	[KEY_F2]            = 0,
	[KEY_F3]            = 0,
	[KEY_F4]            = 0,
	[KEY_F5]            = 0,
	[KEY_F6]            = 0,
	[KEY_F7]            = 0,
	[KEY_F8]            = 0,
	[KEY_F9]            = 0,
	[KEY_F10]           = 0,
	[KEY_F11]           = 0,
	[KEY_F12]           = 0,
	[KEY_LEFT_SHIFT]    = 0,
	[KEY_LEFT_CONTROL]  = 0,
	[KEY_LEFT_ALT]      = 0,
	[KEY_LEFT_SUPER]    = 0,
	[KEY_RIGHT_SHIFT]   = 0,
	[KEY_RIGHT_CONTROL] = 0,
	[KEY_RIGHT_ALT]     = 0,
	[KEY_RIGHT_SUPER]   = 0,
	[KEY_KB_MENU]       = 0,
	[KEY_KP_0]          = 0,
	[KEY_KP_1]          = 0,
	[KEY_KP_2]          = 0,
	[KEY_KP_3]          = 0,
	[KEY_KP_4]          = 0,
	[KEY_KP_5]          = 0,
	[KEY_KP_6]          = 0,
	[KEY_KP_7]          = 0,
	[KEY_KP_8]          = 0,
	[KEY_KP_9]          = 0,
	[KEY_KP_DECIMAL]    = 0,
	[KEY_KP_DIVIDE]     = 0,
	[KEY_KP_MULTIPLY]   = 0,
	[KEY_KP_SUBTRACT]   = 0,
	[KEY_KP_ADD]        = 0,
	[KEY_KP_ENTER]      = 0,
	[KEY_KP_EQUAL]      = 0,
};

float KeyboardKeyHoldTimers[] = {
	[KEY_NULL]          = 0,
	[KEY_APOSTROPHE]    = 0,
	[KEY_COMMA]         = 0,
	[KEY_MINUS]         = 0,
	[KEY_PERIOD]        = 0,
	[KEY_SLASH]         = 0,
	[KEY_ZERO]          = 0,
	[KEY_ONE]           = 0,
	[KEY_TWO]           = 0,
	[KEY_THREE]         = 0,
	[KEY_FOUR]          = 0,
	[KEY_FIVE]          = 0,
	[KEY_SIX]           = 0,
	[KEY_SEVEN]         = 0,
	[KEY_EIGHT]         = 0,
	[KEY_NINE]          = 0,
	[KEY_SEMICOLON]     = 0,
	[KEY_EQUAL]         = 0,
	[KEY_A]             = 0,
	[KEY_B]             = 0,
	[KEY_C]             = 0,
	[KEY_D]             = 0,
	[KEY_E]             = 0,
	[KEY_F]             = 0,
	[KEY_G]             = 0,
	[KEY_H]             = 0,
	[KEY_I]             = 0,
	[KEY_J]             = 0,
	[KEY_K]             = 0,
	[KEY_L]             = 0,
	[KEY_M]             = 0,
	[KEY_N]             = 0,
	[KEY_O]             = 0,
	[KEY_P]             = 0,
	[KEY_Q]             = 0,
	[KEY_R]             = 0,
	[KEY_S]             = 0,
	[KEY_T]             = 0,
	[KEY_U]             = 0,
	[KEY_V]             = 0,
	[KEY_W]             = 0,
	[KEY_X]             = 0,
	[KEY_Y]             = 0,
	[KEY_Z]             = 0,
	[KEY_LEFT_BRACKET]  = 0,
	[KEY_BACKSLASH]     = 0,
	[KEY_RIGHT_BRACKET] = 0,
	[KEY_GRAVE]         = 0,
	[KEY_SPACE]         = 0,
	[KEY_ESCAPE]        = 0,
	[KEY_ENTER]         = 0,
	[KEY_TAB]           = 0,
	[KEY_BACKSPACE]     = 0,
	[KEY_INSERT]        = 0,
	[KEY_DELETE]        = 0,
	[KEY_RIGHT]         = 0,
	[KEY_LEFT]          = 0,
	[KEY_DOWN]          = 0,
	[KEY_UP]            = 0,
	[KEY_PAGE_UP]       = 0,
	[KEY_PAGE_DOWN]     = 0,
	[KEY_HOME]          = 0,
	[KEY_END]           = 0,
	[KEY_CAPS_LOCK]     = 0,
	[KEY_SCROLL_LOCK]   = 0,
	[KEY_NUM_LOCK]      = 0,
	[KEY_PRINT_SCREEN]  = 0,
	[KEY_PAUSE]         = 0,
	[KEY_F1]            = 0,
	[KEY_F2]            = 0,
	[KEY_F3]            = 0,
	[KEY_F4]            = 0,
	[KEY_F5]            = 0,
	[KEY_F6]            = 0,
	[KEY_F7]            = 0,
	[KEY_F8]            = 0,
	[KEY_F9]            = 0,
	[KEY_F10]           = 0,
	[KEY_F11]           = 0,
	[KEY_F12]           = 0,
	[KEY_LEFT_SHIFT]    = 0,
	[KEY_LEFT_CONTROL]  = 0,
	[KEY_LEFT_ALT]      = 0,
	[KEY_LEFT_SUPER]    = 0,
	[KEY_RIGHT_SHIFT]   = 0,
	[KEY_RIGHT_CONTROL] = 0,
	[KEY_RIGHT_ALT]     = 0,
	[KEY_RIGHT_SUPER]   = 0,
	[KEY_KB_MENU]       = 0,
	[KEY_KP_0]          = 0,
	[KEY_KP_1]          = 0,
	[KEY_KP_2]          = 0,
	[KEY_KP_3]          = 0,
	[KEY_KP_4]          = 0,
	[KEY_KP_5]          = 0,
	[KEY_KP_6]          = 0,
	[KEY_KP_7]          = 0,
	[KEY_KP_8]          = 0,
	[KEY_KP_9]          = 0,
	[KEY_KP_DECIMAL]    = 0,
	[KEY_KP_DIVIDE]     = 0,
	[KEY_KP_MULTIPLY]   = 0,
	[KEY_KP_SUBTRACT]   = 0,
	[KEY_KP_ADD]        = 0,
	[KEY_KP_ENTER]      = 0,
	[KEY_KP_EQUAL]      = 0,
};

float KeyboardKeyUpTimers[] = {
	[KEY_NULL]          = 0,
	[KEY_APOSTROPHE]    = 0,
	[KEY_COMMA]         = 0,
	[KEY_MINUS]         = 0,
	[KEY_PERIOD]        = 0,
	[KEY_SLASH]         = 0,
	[KEY_ZERO]          = 0,
	[KEY_ONE]           = 0,
	[KEY_TWO]           = 0,
	[KEY_THREE]         = 0,
	[KEY_FOUR]          = 0,
	[KEY_FIVE]          = 0,
	[KEY_SIX]           = 0,
	[KEY_SEVEN]         = 0,
	[KEY_EIGHT]         = 0,
	[KEY_NINE]          = 0,
	[KEY_SEMICOLON]     = 0,
	[KEY_EQUAL]         = 0,
	[KEY_A]             = 0,
	[KEY_B]             = 0,
	[KEY_C]             = 0,
	[KEY_D]             = 0,
	[KEY_E]             = 0,
	[KEY_F]             = 0,
	[KEY_G]             = 0,
	[KEY_H]             = 0,
	[KEY_I]             = 0,
	[KEY_J]             = 0,
	[KEY_K]             = 0,
	[KEY_L]             = 0,
	[KEY_M]             = 0,
	[KEY_N]             = 0,
	[KEY_O]             = 0,
	[KEY_P]             = 0,
	[KEY_Q]             = 0,
	[KEY_R]             = 0,
	[KEY_S]             = 0,
	[KEY_T]             = 0,
	[KEY_U]             = 0,
	[KEY_V]             = 0,
	[KEY_W]             = 0,
	[KEY_X]             = 0,
	[KEY_Y]             = 0,
	[KEY_Z]             = 0,
	[KEY_LEFT_BRACKET]  = 0,
	[KEY_BACKSLASH]     = 0,
	[KEY_RIGHT_BRACKET] = 0,
	[KEY_GRAVE]         = 0,
	[KEY_SPACE]         = 0,
	[KEY_ESCAPE]        = 0,
	[KEY_ENTER]         = 0,
	[KEY_TAB]           = 0,
	[KEY_BACKSPACE]     = 0,
	[KEY_INSERT]        = 0,
	[KEY_DELETE]        = 0,
	[KEY_RIGHT]         = 0,
	[KEY_LEFT]          = 0,
	[KEY_DOWN]          = 0,
	[KEY_UP]            = 0,
	[KEY_PAGE_UP]       = 0,
	[KEY_PAGE_DOWN]     = 0,
	[KEY_HOME]          = 0,
	[KEY_END]           = 0,
	[KEY_CAPS_LOCK]     = 0,
	[KEY_SCROLL_LOCK]   = 0,
	[KEY_NUM_LOCK]      = 0,
	[KEY_PRINT_SCREEN]  = 0,
	[KEY_PAUSE]         = 0,
	[KEY_F1]            = 0,
	[KEY_F2]            = 0,
	[KEY_F3]            = 0,
	[KEY_F4]            = 0,
	[KEY_F5]            = 0,
	[KEY_F6]            = 0,
	[KEY_F7]            = 0,
	[KEY_F8]            = 0,
	[KEY_F9]            = 0,
	[KEY_F10]           = 0,
	[KEY_F11]           = 0,
	[KEY_F12]           = 0,
	[KEY_LEFT_SHIFT]    = 0,
	[KEY_LEFT_CONTROL]  = 0,
	[KEY_LEFT_ALT]      = 0,
	[KEY_LEFT_SUPER]    = 0,
	[KEY_RIGHT_SHIFT]   = 0,
	[KEY_RIGHT_CONTROL] = 0,
	[KEY_RIGHT_ALT]     = 0,
	[KEY_RIGHT_SUPER]   = 0,
	[KEY_KB_MENU]       = 0,
	[KEY_KP_0]          = 0,
	[KEY_KP_1]          = 0,
	[KEY_KP_2]          = 0,
	[KEY_KP_3]          = 0,
	[KEY_KP_4]          = 0,
	[KEY_KP_5]          = 0,
	[KEY_KP_6]          = 0,
	[KEY_KP_7]          = 0,
	[KEY_KP_8]          = 0,
	[KEY_KP_9]          = 0,
	[KEY_KP_DECIMAL]    = 0,
	[KEY_KP_DIVIDE]     = 0,
	[KEY_KP_MULTIPLY]   = 0,
	[KEY_KP_SUBTRACT]   = 0,
	[KEY_KP_ADD]        = 0,
	[KEY_KP_ENTER]      = 0,
	[KEY_KP_EQUAL]      = 0,
};

float AndroidKeyPressTimers[] = {
	[KEY_BACK]        = 0,
	[KEY_MENU]        = 0,
	[KEY_VOLUME_UP]   = 0,
	[KEY_VOLUME_DOWN] = 0,
};

float AndroidKeyHoldTimers[] = {
	[KEY_BACK]        = 0,
	[KEY_MENU]        = 0,
	[KEY_VOLUME_UP]   = 0,
	[KEY_VOLUME_DOWN] = 0,
};

float AndroidKeyUpTimers[] = {
	[KEY_BACK]        = 0,
	[KEY_MENU]        = 0,
	[KEY_VOLUME_UP]   = 0,
	[KEY_VOLUME_DOWN] = 0,
};

float MouseButtonPressTimers[] = {
	[MOUSE_BUTTON_LEFT]  = 0, [MOUSE_BUTTON_RIGHT]   = 0, [MOUSE_BUTTON_MIDDLE] = 0, [MOUSE_BUTTON_SIDE] = 0,
	[MOUSE_BUTTON_EXTRA] = 0, [MOUSE_BUTTON_FORWARD] = 0, [MOUSE_BUTTON_BACK]   = 0,
};

float MouseButtonHoldTimers[] = {
	[MOUSE_BUTTON_LEFT]  = 0, [MOUSE_BUTTON_RIGHT]   = 0, [MOUSE_BUTTON_MIDDLE] = 0, [MOUSE_BUTTON_SIDE] = 0,
	[MOUSE_BUTTON_EXTRA] = 0, [MOUSE_BUTTON_FORWARD] = 0, [MOUSE_BUTTON_BACK]   = 0,
};

float MouseButtonUpTimers[] = {
	[MOUSE_BUTTON_LEFT]  = 0, [MOUSE_BUTTON_RIGHT]   = 0, [MOUSE_BUTTON_MIDDLE] = 0, [MOUSE_BUTTON_SIDE] = 0,
	[MOUSE_BUTTON_EXTRA] = 0, [MOUSE_BUTTON_FORWARD] = 0, [MOUSE_BUTTON_BACK]   = 0,
};

float GamepadButtonPressTimers[][MAX_GAMEPADS] = {
	[GAMEPAD_BUTTON_UNKNOWN]          = {0}, [GAMEPAD_BUTTON_LEFT_FACE_UP]    = {0},
	[GAMEPAD_BUTTON_LEFT_FACE_RIGHT]  = {0}, [GAMEPAD_BUTTON_LEFT_FACE_DOWN]  = {0},
	[GAMEPAD_BUTTON_LEFT_FACE_LEFT]   = {0}, [GAMEPAD_BUTTON_RIGHT_FACE_UP]   = {0},
	[GAMEPAD_BUTTON_RIGHT_FACE_RIGHT] = {0}, [GAMEPAD_BUTTON_RIGHT_FACE_DOWN] = {0},
	[GAMEPAD_BUTTON_RIGHT_FACE_LEFT]  = {0}, [GAMEPAD_BUTTON_LEFT_TRIGGER_1]  = {0},
	[GAMEPAD_BUTTON_LEFT_TRIGGER_2]   = {0}, [GAMEPAD_BUTTON_RIGHT_TRIGGER_1] = {0},
	[GAMEPAD_BUTTON_RIGHT_TRIGGER_2]  = {0}, [GAMEPAD_BUTTON_MIDDLE_LEFT]     = {0},
	[GAMEPAD_BUTTON_MIDDLE]           = {0}, [GAMEPAD_BUTTON_MIDDLE_RIGHT]    = {0},
	[GAMEPAD_BUTTON_LEFT_THUMB]       = {0}, [GAMEPAD_BUTTON_RIGHT_THUMB]     = {0},
};

float GamepadButtonHoldTimers[][MAX_GAMEPADS] = {
	[GAMEPAD_BUTTON_UNKNOWN]          = {0}, [GAMEPAD_BUTTON_LEFT_FACE_UP]    = {0},
	[GAMEPAD_BUTTON_LEFT_FACE_RIGHT]  = {0}, [GAMEPAD_BUTTON_LEFT_FACE_DOWN]  = {0},
	[GAMEPAD_BUTTON_LEFT_FACE_LEFT]   = {0}, [GAMEPAD_BUTTON_RIGHT_FACE_UP]   = {0},
	[GAMEPAD_BUTTON_RIGHT_FACE_RIGHT] = {0}, [GAMEPAD_BUTTON_RIGHT_FACE_DOWN] = {0},
	[GAMEPAD_BUTTON_RIGHT_FACE_LEFT]  = {0}, [GAMEPAD_BUTTON_LEFT_TRIGGER_1]  = {0},
	[GAMEPAD_BUTTON_LEFT_TRIGGER_2]   = {0}, [GAMEPAD_BUTTON_RIGHT_TRIGGER_1] = {0},
	[GAMEPAD_BUTTON_RIGHT_TRIGGER_2]  = {0}, [GAMEPAD_BUTTON_MIDDLE_LEFT]     = {0},
	[GAMEPAD_BUTTON_MIDDLE]           = {0}, [GAMEPAD_BUTTON_MIDDLE_RIGHT]    = {0},
	[GAMEPAD_BUTTON_LEFT_THUMB]       = {0}, [GAMEPAD_BUTTON_RIGHT_THUMB]     = {0},
};

float GamepadButtonUpTimers[][MAX_GAMEPADS] = {
	[GAMEPAD_BUTTON_UNKNOWN]          = {0}, [GAMEPAD_BUTTON_LEFT_FACE_UP]    = {0},
	[GAMEPAD_BUTTON_LEFT_FACE_RIGHT]  = {0}, [GAMEPAD_BUTTON_LEFT_FACE_DOWN]  = {0},
	[GAMEPAD_BUTTON_LEFT_FACE_LEFT]   = {0}, [GAMEPAD_BUTTON_RIGHT_FACE_UP]   = {0},
	[GAMEPAD_BUTTON_RIGHT_FACE_RIGHT] = {0}, [GAMEPAD_BUTTON_RIGHT_FACE_DOWN] = {0},
	[GAMEPAD_BUTTON_RIGHT_FACE_LEFT]  = {0}, [GAMEPAD_BUTTON_LEFT_TRIGGER_1]  = {0},
	[GAMEPAD_BUTTON_LEFT_TRIGGER_2]   = {0}, [GAMEPAD_BUTTON_RIGHT_TRIGGER_1] = {0},
	[GAMEPAD_BUTTON_RIGHT_TRIGGER_2]  = {0}, [GAMEPAD_BUTTON_MIDDLE_LEFT]     = {0},
	[GAMEPAD_BUTTON_MIDDLE]           = {0}, [GAMEPAD_BUTTON_MIDDLE_RIGHT]    = {0},
	[GAMEPAD_BUTTON_LEFT_THUMB]       = {0}, [GAMEPAD_BUTTON_RIGHT_THUMB]     = {0},
};

static const uint KeyboardKeyCount   = sizeof(KeyboardKeyPressTimers) / sizeof(*KeyboardKeyPressTimers);
static const uint AndroidKeyCount    = sizeof(AndroidKeyPressTimers) / sizeof(*AndroidKeyPressTimers);
static const uint MouseButtonCount   = sizeof(MouseButtonPressTimers) / sizeof(*MouseButtonPressTimers);
static const uint GamepadButtonCount = sizeof(GamepadButtonPressTimers) / sizeof(*GamepadButtonPressTimers);

void UpdateLuEngine() {
	frameCount++;
	UpdateInputsTimers();
	UpdateCameraLu(&globalCamera);
}

void UpdateInputsTimers() {
	float currTime  = GetTime();
	float deltaTime = GetFrameTime();

	for (uint i = 0; i < KeyboardKeyCount; i++) {
		KeyboardKeyPressTimers[i] += deltaTime;
		KeyboardKeyHoldTimers[i]  += deltaTime;
		KeyboardKeyUpTimers[i]    += deltaTime;

		if (IsKeyPressed(i)) {
			KeyboardKeyPressTimers[i] = 0;
			KeyboardKeyHoldTimers[i]  = 0;
			KeyboardKeyUpTimers[i]    = 0;
		}
		if (IsKeyDown(i)) {
			KeyboardKeyUpTimers[i] = 0;
		}
		if (IsKeyReleased(i)) {
			KeyboardKeyUpTimers[i]   = 0;
			KeyboardKeyHoldTimers[i] = 0;
		}
		if (IsKeyUp(i)) {
			KeyboardKeyHoldTimers[i] = 0;
		}
	}

	for (uint i = 0; i < AndroidKeyCount; i++) {
		AndroidKeyPressTimers[i] += deltaTime;
		AndroidKeyHoldTimers[i]  += deltaTime;
		AndroidKeyUpTimers[i]    += deltaTime;

		if (IsKeyPressed(i)) {
			AndroidKeyPressTimers[i] = 0;
			AndroidKeyHoldTimers[i]  = 0;
			AndroidKeyUpTimers[i]    = 0;
		}
		if (IsKeyDown(i)) {
			AndroidKeyUpTimers[i] = 0;
		}
		if (IsKeyReleased(i)) {
			AndroidKeyUpTimers[i]   = 0;
			AndroidKeyHoldTimers[i] = 0;
		}
		if (IsKeyUp(i)) {
			AndroidKeyHoldTimers[i] = 0;
		}
	}

	for (uint i = 0; i < MouseButtonCount; i++) {
		MouseButtonPressTimers[i] += deltaTime;
		MouseButtonHoldTimers[i]  += deltaTime;
		MouseButtonUpTimers[i]    += deltaTime;

		if (IsMouseButtonPressed(i)) {
			MouseButtonPressTimers[i] = 0;
			MouseButtonHoldTimers[i]  = 0;
			MouseButtonUpTimers[i]    = 0;
		}
		if (IsMouseButtonDown(i)) {
			MouseButtonUpTimers[i] = 0;
		}
		if (IsMouseButtonReleased(i)) {
			MouseButtonUpTimers[i]   = 0;
			MouseButtonHoldTimers[i] = 0;
		}
		if (IsMouseButtonUp(i)) {
			MouseButtonHoldTimers[i] = 0;
		}
	}

	for (uint i = 0; i < GamepadButtonCount; i++) {
		for (uint padNum = 0; padNum < MAX_GAMEPADS; padNum++) {
			GamepadButtonPressTimers[i][padNum] += deltaTime;
			GamepadButtonHoldTimers[i][padNum]  += deltaTime;
			GamepadButtonUpTimers[i][padNum]    += deltaTime;

			if (IsGamepadButtonPressed(padNum, i)) {
				GamepadButtonPressTimers[i][padNum] = 0;
				GamepadButtonHoldTimers[i][padNum]  = 0;
				GamepadButtonUpTimers[i][padNum]    = 0;
			}
			if (IsGamepadButtonDown(padNum, i)) {
				GamepadButtonUpTimers[i][padNum] = 0;
			}
			if (IsGamepadButtonReleased(padNum, i)) {
				GamepadButtonUpTimers[i][padNum]   = 0;
				GamepadButtonHoldTimers[i][padNum] = 0;
			}
			if (IsGamepadButtonUp(padNum, i)) {
				GamepadButtonHoldTimers[i][padNum] = 0;
			}
		}
	}
}

void UpdateCameraLu(Camera2D* cam) {
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 camMove     = GetMouseDelta();
		        camMove     = Vector2Scale(camMove, -1 / cam->zoom);
		        cam->target = Vector2Add(cam->target, camMove);
	}

	float zoomAmount = GetMouseWheelMove() * 0.15f;

	if (zoomAmount != 0) {
		Vector2 mouseBefore  = GetScreenToWorld2D(GetMousePosition(), *cam);
		        cam->zoom   *= 1 + zoomAmount;
		Vector2 mouseAfter   = GetScreenToWorld2D(GetMousePosition(), *cam);
		Vector2 mouseDiff    = Vector2Subtract(mouseBefore, mouseAfter);
		        cam->target  = Vector2Add(cam->target, mouseDiff);
	}
}

char GetCharFromKey(int key) {
	if (key >= KEY_KP_0 && key <= KEY_KP_9) return '0' + (key - KEY_KP_0);
	if (key >= KEY_A && key <= KEY_Z) return (key - KEY_A) + ((IsShiftDown) ? 'A' : 'a');
	if (key >= KEY_ZERO && key <= KEY_NINE) return ((IsShiftDown) ? "!@#$%^&*()" : "0123456789")[key - KEY_ZERO];

	if (key == KEY_COMMA) return (IsShiftDown) ? '<' : ',';
	if (key == KEY_PERIOD) return (IsShiftDown) ? '>': '.';
	if (key == KEY_SLASH) return (IsShiftDown) ? '?' : '/';

	if (key == KEY_SEMICOLON) return (IsShiftDown) ? ':' : ';';
	if (key == KEY_APOSTROPHE) return (IsShiftDown) ? '"': '\'';

	if (key == KEY_LEFT_BRACKET) return (IsShiftDown) ? '{' : '[';
	if (key == KEY_RIGHT_BRACKET) return (IsShiftDown) ? '}': ']';
	if (key == KEY_BACKSLASH) return (IsShiftDown) ? '|'    : '\\';

	if (key == KEY_GRAVE) return (IsShiftDown) ? '~': '`';

	if (key == KEY_MINUS) return (IsShiftDown) ? '_': '-';
	if (key == KEY_EQUAL) return (IsShiftDown) ? '+': '=';

	if (key == KEY_KP_DECIMAL) return '.';
	if (key == KEY_KP_DIVIDE) return '/';
	if (key == KEY_KP_MULTIPLY) return '*';
	if (key == KEY_KP_SUBTRACT) return '-';
	if (key == KEY_KP_ADD) return '+';
	if (key == KEY_KP_EQUAL) return '=';

	return 0;  // not a querty key
}
