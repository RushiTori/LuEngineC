#include "Button.h"

Button ButtonCreate(float x, float y, float w, float h) {
	Button temp = EmptyButton;
	temp.body = (Rectangle){.x = x, .y = y, .width = w, .height = h};
	return temp;
}

Button ButtonCreateV(Vector2 pos, float w, float h) {
	Button temp = EmptyButton;
	temp.body = (Rectangle){.x = pos.x, .y = pos.y, .width = w, .height = h};
	return temp;
}

Button ButtonCreateRec(Rectangle rec) {
	Button temp = EmptyButton;
	temp.body = rec;
	return temp;
}

void ButtonSetOnClickAction(Button* button, void (*onClickAction)(void)) { button->onClick = onClickAction; }

void ButtonSetOnHoldAction(Button* button, void (*onHoldAction)(void)) { button->onHold = onHoldAction; }

void ButtonSetOnReleaseAction(Button* button, void (*onReleaseAction)(void)) { button->onRelease = onReleaseAction; }

void ButtonSetPos(Button* button, Vector2 pos) {
	button->body.x = pos.x;
	button->body.y = pos.y;
}

void ButtonMove(Button* button, Vector2 vel) {
	button->body.x += vel.x;
	button->body.y += vel.y;
}

void ButtonSetSizes(Button* button, float w, float h) {
	button->body.width += w;
	button->body.height += h;
}

void ButtonSetMouseButton(Button* button, int mouseButton) { button->mouseButton = mouseButton; }

void ButtonUpdate(Button* button, Vector2 mousePos) {
	button->isMouseOn = button->isActive && CheckCollisionPointRec(mousePos, button->body);
	if (!button->isMouseOn) return;

	if (IsMouseButtonPressed(button->mouseButton)) {
		if (button->onClick) button->onClick();
	} else if (IsMouseButtonDown(button->mouseButton)) {
		if (button->onHold) button->onHold();
	} else if (IsMouseButtonReleased(button->mouseButton)) {
		if (button->onRelease) button->onRelease();
	}
}

void ButtonDisplay(const Button* button) {
	DrawRectangleRec(button->body, GRAY);
	if (button->isMouseOn) DrawRectangleRec(button->body, Fade(WHITE, 0.2));
	if (!button->isActive) DrawRectangleRec(button->body, Fade(BLACK, 0.2));
	DrawRectangleLinesEx(button->body, 1, BLACK);
}
