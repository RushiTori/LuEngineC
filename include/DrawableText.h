#ifndef DRAWABLE_TEXT_H
#define DRAWABLE_TEXT_H

#include <LuLib/LuString.h>

#include "EngineUtils.h"

typedef enum TextAlign {
	ALIGN_CENTER = 0,
	ALIGN_LEFT   = 1,
	ALIGN_RIGHT  = 2,
	ALIGN_TOP    = 3,
	ALIGN_BOTTOM = 4,
} __attribute__((__packed__)) TextAlign;

typedef struct DrawableText {
	string text;
	float textSize;
	Color textCol;
	TextAlign textAlignX, textAlignY;

	Rectangle body;
	TextAlign bodyAlignX, bodyAlignY;
} DrawableText;

float GetAlignedOffset(float left, float center, float right, TextAlign align);

#define EmptyDrawableText                                                                               \
	(DrawableText) {                                                                                    \
		.text = NULL, .textSize = 10, .textCol = (Color){.r = 255, .g = 255, .b = 255, .a = 255},       \
		.textAlignX = ALIGN_LEFT, .textAlignY = ALIGN_TOP,                                              \
		.body = (Rectangle){.x = 0, .y = 0, .width = 1920, .height = 1080}, .bodyAlignX = ALIGN_CENTER, \
		.bodyAlignY = ALIGN_CENTER                                                                      \
	}

DrawableText DrawableTextCreate(const string text, float textSize, Color textCol);

void DrawableTextSetText(DrawableText* drawableText, const string text);
void DrawableTextSetTextSize(DrawableText* drawableText, float textSize);
void DrawableTextSetTextCol(DrawableText* drawableText, Color textCol);
void DrawableTextSetTextAligns(DrawableText* drawableText, TextAlign alignX, TextAlign alignY);

void DrawableTextSetBody(DrawableText* drawableText, float x, float y, float w, float h);
void DrawableTextSetBodyV(DrawableText* drawableText, Vector2 pos, float w, float h);
void DrawableTextSetBodyRec(DrawableText* drawableText, Rectangle rec);

void DrawableTextSetBodyPos(DrawableText* drawableText, Vector2 pos);
void DrawableTextSetBodySizes(DrawableText* drawableText, float w, float h);

void DrawableTextSetBodyAligns(DrawableText* drawableText, TextAlign alignX, TextAlign alignY);

void DrawableTextDisplay(DrawableText* drawableText);

#endif	// DRAWABLE_TEXT_H
