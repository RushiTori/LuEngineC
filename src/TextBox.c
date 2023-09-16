#include "TextBox.h"

TextBox TextBoxCreate(float x, float y, float w, float h, uint maxCharCount, const string alphabet) {
	TextBox temp = EmptyTextBox;

	temp.alphabet = alphabet;
	arr_Reserve(&temp.content, maxCharCount);
	arr_Push(&temp.content, "\0", 1);
	temp.maxCharCount = maxCharCount;
	temp.body = (Rectangle){.x = x, .y = y, .width = w, .height = h};

	return temp;
}

TextBox TextBoxCreateV(Vector2 pos, float w, float h, uint maxCharCount, const string alphabet) {
	TextBox temp = EmptyTextBox;

	temp.alphabet = alphabet;
	arr_Reserve(&temp.content, maxCharCount);
	arr_Push(&temp.content, "\0", 1);
	temp.maxCharCount = maxCharCount;
	temp.body = (Rectangle){.x = pos.x, .y = pos.y, .width = w, .height = h};

	return temp;
}

TextBox TextBoxCreateRec(Rectangle rec, uint maxCharCount, const string alphabet) {
	TextBox temp = EmptyTextBox;

	temp.alphabet = alphabet;
	arr_Reserve(&temp.content, maxCharCount);
	arr_Push(&temp.content, "\0", 1);
	temp.maxCharCount = maxCharCount;
	temp.body = rec;

	return temp;
}

string TextBoxGetSelection(TextBox* box) {
	if (box->selectCursor < 0) return NULL;
	if (box->writingCursor == box->selectCursor) return NULL;

	uint selectIdx = min(box->writingCursor, box->selectCursor);
	uint selectLen = max(box->writingCursor, box->selectCursor) - selectIdx;
	string selection = calloc(selectLen + 1, sizeof(char));
	memcpy(selection, arr_Get(&box->content, selectIdx), selectLen);

	return selection;
}

void TextBoxMouseInteract(TextBox* box, Vector2 mousePos) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		box->isFocused = CheckCollisionPointRec(mousePos, box->body);
	}
	if (!box->isFocused) {
		box->selectCursor = -1;
		return;
	}
	// WIP
}

void TextBoxEraseChars(TextBox* box, uint eraseIdx, uint eraseCount) {
	arr_Remove(&box->content, eraseIdx, eraseCount, false);
	box->selectCursor = -1;
	box->writingCursor = eraseIdx;
}

bool TextBoxEraseSelection(TextBox* box) {
	if (box->selectCursor < 0) return false;
	uint eraseIdx = min(box->writingCursor, box->selectCursor);
	uint eraseCount = max(box->writingCursor, box->selectCursor) - eraseIdx;
	TextBoxEraseChars(box, eraseIdx, eraseCount);
	return true;
}

void TextBoxAddChars(TextBox* box, const char* str, uint charCount) {
	TextBoxEraseSelection(box);
	for (uint i = 0; i < charCount; i++) {
		if (!str_Contains(box->alphabet, str[i])) continue;
		if (box->content.elementCount > box->maxCharCount) break;
		arr_Insert(&box->content, box->writingCursor++, str + i, 1);
	}
}

bool RepeatedKeyInput(int key, float timeThreshold, int perSecond) {
	if (IsKeyPressed(key)) return true;
	if (!IsKeyDown(key)) return false;
	if (KeyboardKeyHoldTimers[key] < timeThreshold) return false;

	if (GetCustomFrameCount(KeyboardKeyHoldTimers[key], 1.0 / perSecond) ==
		GetCustomFrameCount(KeyboardKeyHoldTimers[key] - GetFrameTime(), 1.0 / perSecond)) {
		return false;
	}

	return true;
}

void TextBoxUpdate(TextBox* box, Vector2 mousePos) {
	if (!box->isActive) {
		box->isFocused = false;
		return;
	}

	TextBoxMouseInteract(box, mousePos);

	if (!box->isFocused) return;

	for (int key = GetKeyPressed(); key; key = GetKeyPressed()) {
		box->writingCursor = clamp(box->writingCursor, 0, box->content.elementCount - 1);
		if ((key == KEY_HOME) || (key == KEY_PAGE_UP)) {
			if (IsShiftDown) {
				if (box->selectCursor < 0) box->selectCursor = box->writingCursor;
			} else {
				box->selectCursor = -1;
			}
			box->writingCursor = 0;
		} else if ((key == KEY_END) || key == (KEY_PAGE_DOWN)) {
			if (IsShiftDown) {
				if (box->selectCursor < 0) box->selectCursor = box->writingCursor;
			} else {
				box->selectCursor = -1;
			}
			box->writingCursor = box->content.elementCount - 1;
		} else if (key == KEY_C) {
			if (IsCtrlDown) {
				string selection = TextBoxGetSelection(box);
				if (selection) {
					SetClipboardText(selection);
					free(selection);
				}
				continue;
			}
		} else if (key == KEY_X) {
			if (IsCtrlDown) {
				string selection = TextBoxGetSelection(box);
				if (selection) {
					SetClipboardText(selection);
					TextBoxEraseSelection(box);
					free(selection);
				}
				continue;
			}
		} else if (key == KEY_V) {
			if (IsCtrlDown) {
				const char* clipText = GetClipboardText();
				TextBoxAddChars(box, clipText, strlen(clipText));
				continue;
			}
		}

		char c = GetCharFromKey(key);
		if (c) TextBoxAddChars(box, &c, 1);
	}

	if (box->content.elementCount <= 1) return;

	if (RepeatedKeyInput(KEY_BACKSPACE, 0.160, 16)) {
		if (!TextBoxEraseSelection(box)) TextBoxEraseChars(box, --box->writingCursor, 1);
	} else if (RepeatedKeyInput(KEY_DELETE, 0.160, 16)) {
		if (!TextBoxEraseSelection(box) && (box->writingCursor < (int)box->content.elementCount - 1)) {
			TextBoxEraseChars(box, box->writingCursor, 1);
		}
	}

	if (RepeatedKeyInput(KEY_LEFT, 0.160, 16)) {
		if (IsShiftDown) {
			if (box->selectCursor < 0) box->selectCursor = box->writingCursor;
			box->writingCursor--;
		} else if (box->selectCursor >= 0) {
			box->writingCursor = min(box->writingCursor, box->selectCursor);
			box->selectCursor = -1;
		} else {
			box->writingCursor--;
		}
	} else if (RepeatedKeyInput(KEY_RIGHT, 0.160, 16)) {
		if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
			if (box->selectCursor < 0) box->selectCursor = box->writingCursor;
			box->writingCursor++;
		} else if (box->selectCursor >= 0) {
			box->writingCursor = max(box->writingCursor, box->selectCursor);
			box->selectCursor = -1;
		} else {
			box->writingCursor++;
		}
	}

	box->writingCursor = clamp(box->writingCursor, 0, box->content.elementCount - 1);

	if (box->writingCursor == box->selectCursor) box->selectCursor = -1;
}

float TextWidth(char* text, uint n, float textSize) {
	if (!text) return 0;

	n = min(n, strlen(text));
	if (n == strlen(text)) return MeasureTextEx(GetFontDefault(), text, textSize, (int)(textSize / 10)).x;

	char temp = text[n];
	text[n] = '\0';
	float result = MeasureTextEx(GetFontDefault(), text, textSize, (int)(textSize / 10)).x;
	text[n] = temp;

	return result;
}

void TextBoxDisplayCursor(TextBox* box) {
	float cursorThickness = MeasureTextEx(GetFontDefault(), "I", box->body.height, box->body.height / 10).x;
	cursorThickness /= 2;

	float cursorSize = box->body.height * 0.90;
	cursorSize *= mapValue(sin(GetTime() * 2) * sin(GetTime() * 2), 0, 1, 0.15, 1);

	float cursorY = box->body.y + (box->body.height * 0.525);

	float cursorX = box->body.x + TextWidth(box->content.data, box->writingCursor, box->body.height);

	DrawRectangle(cursorX + cursorThickness / 2, cursorY - cursorSize / 2, cursorThickness, cursorSize, BLACK);
}

void TextBoxDisplay(TextBox* box) {
	// WIP
	DrawRectangleRec(box->body, DARKGRAY);
	if (box->isFocused) DrawRectangleRec(box->body, Fade(WHITE, 0.2));

	if (!box->content.elementCount) return;
	DrawText((char*)box->content.data, box->body.x, box->body.y, box->body.height, RAYWHITE);

	if (!box->isFocused) return;
	TextBoxDisplayCursor(box);
	if (box->selectCursor < 0) return;
	float cursorThickness = TextWidth("I", 1, box->body.height);
	float cursorX = box->body.x + TextWidth(box->content.data, box->writingCursor, box->body.height);
	float selectX = box->body.x + TextWidth(box->content.data, box->selectCursor, box->body.height);
	DrawRectangle(min(cursorX, selectX), box->body.y + box->body.height * 0.05,
				  max(cursorX, selectX) - min(cursorX, selectX), box->body.height * 0.9, Fade(PINK, 0.2));
}
