#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <LuLib/LuUtils.h>
#include <rayconfig.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#define IsMouseButtonDown(button) (IsMouseButtonDown(button) && !IsMouseButtonPressed(button))
#define IsMouseButtonUp(button) (IsMouseButtonUp(button) && !IsMouseButtonReleased(button))

#define IsKeyDown(key) (IsKeyDown(key) && !IsKeyPressed(key))
#define IsKeyUp(key) (IsKeyUp(key) && !IsKeyReleased(key))

#define IsShiftPressed (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT))
#define IsShiftDown (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
#define IsShiftReleased (IsKeyReleased(KEY_LEFT_SHIFT) || IsKeyReleased(KEY_RIGHT_SHIFT))
#define IsShiftUp (IsKeyUp(KEY_LEFT_SHIFT) && IsKeyUp(KEY_RIGHT_SHIFT))

#define IsCtrlPressed (IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL))
#define IsCtrlDown (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
#define IsCtrlReleased (IsKeyReleased(KEY_LEFT_CONTROL) || IsKeyReleased(KEY_RIGHT_CONTROL))
#define IsCtrlUp (IsKeyUp(KEY_LEFT_CONTROL) && IsKeyUp(KEY_RIGHT_CONTROL))

#define IsGamepadButtonDown(gamepad, button) \
	(IsGamepadButtonDown(gamepad, button) && !IsGamepadButtonPressed(gamepad, button))

#define IsGamepadButtonUp(gamepad, button) \
	(IsGamepadButtonUp(gamepad, button) && !IsGamepadButtonReleased(gamepad, button))

#define GetCustomFrameCount(timer, frameTime) ((int)((timer) / (frameTime)))
#define DoEveryNFrame(n) ((((int)(GetTime() / GetFrameTime())) % (n)) == 0)

extern string ProjectTitle;
extern uint width;
extern uint height;
extern ulong frameCount;
extern Camera2D globalCamera;

extern float KeyboardKeyPressTimers[];
extern float KeyboardKeyHoldTimers[];
extern float KeyboardKeyUpTimers[];

extern float AndroidKeyPressTimers[];
extern float AndroidKeyHoldTimers[];
extern float AndroidKeyUpTimers[];

extern float MouseButtonPressTimers[];
extern float MouseButtonHoldTimers[];
extern float MouseButtonUpTimers[];

extern float GamepadButtonPressTimers[][MAX_GAMEPADS];
extern float GamepadButtonHoldTimers[][MAX_GAMEPADS];
extern float GamepadButtonUpTimers[][MAX_GAMEPADS];

void UpdateLuEngine();

void UpdateInputsTimers();

void UpdateCameraLu(Camera2D* cam);

char GetCharFromKey(int key);

#endif	// ENGINE_UTILS_H
