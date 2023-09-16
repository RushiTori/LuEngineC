#ifndef BUTTON_H
#define BUTTON_H

#include "EngineUtils.h"

typedef struct Button {
	void (*onClick)(void);
	void (*onHold)(void);
	void (*onRelease)(void);
	Rectangle body;
	bool isMouseOn;
	bool isActive;
	int mouseButton;
} Button;

#define EmptyButton                                                                                               \
	(Button) {                                                                                                    \
		.onClick = NULL, .onHold = NULL, .onRelease = NULL, .body = {.x = 0, .y = 0, .width = 275, .height = 30}, \
		.isMouseOn = false, .isActive = true, .mouseButton = MOUSE_BUTTON_LEFT                                    \
	}

Button ButtonCreate(float x, float y, float w, float h);
Button ButtonCreateV(Vector2 pos, float w, float h);
Button ButtonCreateRec(Rectangle rec);

void ButtonSetOnClickAction(Button* button, void (*onClickAction)(void));
void ButtonSetOnHoldAction(Button* button, void (*onHoldAction)(void));
void ButtonSetOnReleaseAction(Button* button, void (*onReleaseAction)(void));

void ButtonSetPos(Button* button, Vector2 pos);
void ButtonMove(Button* button, Vector2 vel);
void ButtonSetSizes(Button* button, float w, float h);

void ButtonSetMouseButton(Button* button, int mouseButton);

void ButtonUpdate(Button* button, Vector2 mousePos);
void ButtonDisplay(const Button* button);

#endif	// BUTTON_H
