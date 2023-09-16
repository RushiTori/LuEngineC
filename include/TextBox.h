#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <LuLib/LuArray.h>
#include <LuLib/LuString.h>

#include "EngineUtils.h"

#define TEXTBOX_ASCII_PRINTABLES " !\"#$%&\'()*+,-./0123456789:.<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#define TEXTBOX_LOWERCASE_ALPHABET " abcdefghijklmnopqrstuvwxyz"
#define TEXTBOX_UPPERCASE_ALPHABET " ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define TEXTBOX_WHOLE_ALPHABET TEXTBOX_LOWERCASE_ALPHABET TEXTBOX_UPPERCASE_ALPHABET

#define TEXTBOX_DIGITS " 0123456789"
#define TEXTBOX_ONLY_DIGITS "0123456789"
#define TEXTBOX_MATH_SYMBOLS " ()+-/*^%.,\'"		  // character '\'' is there as a digit separator
#define TEXTBOX_MATHEXPR_SYMBOLS " <=>~()+-/*^%.,\'"  // character '\'' is there as a digit separator
#define TEXTBOX_TEXT_SYMBOLS " ,:;.?!-()\"\'`"
#define TEXTBOX_MISC_SYMBOLS "_|#$&@[]{}\\"
#define TEXTBOX_ALL_SYMBOLS TEXTBOX_MATHEXPR_SYMBOLS TEXTBOX_TEXT_SYMBOLS TEXTBOX_MISC_SYMBOLS

typedef struct TextBox {
	string alphabet;
	Array content;
	uint maxCharCount;
	int selectCursor;
	int writingCursor;

	Rectangle body;
	bool isActive;
	bool isFocused;
} TextBox;

#define EmptyTextBox                                                                                                   \
	(TextBox) {                                                                                                        \
		.alphabet = TEXTBOX_ASCII_PRINTABLES, .content = arr_Create(sizeof(char), 0, false, false), .maxCharCount = 0, \
		.selectCursor = -1, .writingCursor = 0, .body = (Rectangle){.x = 0, .y = 0, .width = 275, .height = 30},       \
		.isActive = true, .isFocused = false                                                                           \
	}

TextBox TextBoxCreate(float x, float y, float w, float h, uint maxCharCount, const string alphabet);
TextBox TextBoxCreateV(Vector2 pos, float w, float h, uint maxCharCount, const string alphabet);
TextBox TextBoxCreateRec(Rectangle rec, uint maxCharCount, const string alphabet);

string TextBoxGetSelection(TextBox* box);

void TextBoxUpdate(TextBox* box, Vector2 mousePos);
void TextBoxDisplay(TextBox* box);

#endif	// TEXT_BOX_H
