#include "DrawableText.h"

DrawableText DrawableTextCreate(const string text, float textSize, Color textCol) {
	DrawableText temp = EmptyDrawableText;
	temp.text = text;
	temp.textSize = textSize;
	temp.textCol = textCol;
	return temp;
}

void DrawableTextSetText(DrawableText* drawableText, const string text) { drawableText->text = text; }

void DrawableTextSetTextSize(DrawableText* drawableText, float textSize) { drawableText->textSize = textSize; }

void DrawableTextSetTextCol(DrawableText* drawableText, Color textCol) { drawableText->textCol = textCol; }

void DrawableTextSetTextAligns(DrawableText* drawableText, TextAlign alignX, TextAlign alignY) {
	drawableText->textAlignX = alignX;
	drawableText->textAlignY = alignY;
}

void DrawableTextSetBody(DrawableText* drawableText, float x, float y, float w, float h) {
	drawableText->body = (Rectangle){.x = x, .y = y, .width = w, .height = h};
}

void DrawableTextSetBodyV(DrawableText* drawableText, Vector2 pos, float w, float h) {
	drawableText->body = (Rectangle){.x = pos.x, .y = pos.y, .width = w, .height = h};
}

void DrawableTextSetBodyRec(DrawableText* drawableText, Rectangle rec) { drawableText->body = rec; }

void DrawableTextSetBodyPos(DrawableText* drawableText, Vector2 pos) {
	drawableText->body.x = pos.x;
	drawableText->body.y = pos.y;
}
void DrawableTextSetBodySizes(DrawableText* drawableText, float w, float h) {
	drawableText->body.width = w;
	drawableText->body.height = h;
}

void DrawableTextSetBodyAligns(DrawableText* drawableText, TextAlign alignX, TextAlign alignY) {
	drawableText->bodyAlignX = alignX;
	drawableText->bodyAlignY = alignY;
}

void DrawableTextDisplay(DrawableText* drawableText) {
	Vector2 drawPos = {
		.x = drawableText->body.x +
			 GetAlignedOffset(0, drawableText->body.width / 2, drawableText->body.width, drawableText->bodyAlignX),

		.y = drawableText->body.y +
			 GetAlignedOffset(0, drawableText->body.height / 2, drawableText->body.height, drawableText->bodyAlignY)};

	// WIP

	Vector2 textMeasures =
		MeasureTextEx(GetFontDefault(), drawableText->text, drawableText->textSize, (int)(drawableText->textSize / 10));
	float lineHeight = textMeasures.y / (str_CountChar(drawableText->text, '\n') + 1);
	
	drawPos.y -= GetAlignedOffset(0, textMeasures.y / 2, textMeasures.y, drawableText->textAlignY);

	string* lines = str_SplitTokens(drawableText->text, "\n");
	uint textLen = strlen(drawableText->text);
	uint lineIdx = 0;
	for (uint i = 0; i < textLen; i++) {
		if (drawableText->text[i] == '\n') {
			drawPos.y += lineHeight;
			continue;
		}
		if (!lines[lineIdx]) break;

		Vector2 lineMeasures =
			MeasureTextEx(GetFontDefault(), lines[lineIdx], drawableText->textSize, (int)(drawableText->textSize / 10));
		Vector2 linePos = drawPos;
		linePos.x -= GetAlignedOffset(0, lineMeasures.x / 2, lineMeasures.x, drawableText->textAlignX);

		DrawTextEx(GetFontDefault(), lines[lineIdx], linePos, drawableText->textSize,
				   (int)(drawableText->textSize / 10), drawableText->textCol);
		drawPos.y += lineHeight;

		i += strlen(lines[lineIdx]);
		free(lines[lineIdx++]);
	}
	free(lines);
}

float GetAlignedOffset(float left, float center, float right, TextAlign align) {
	if ((align == ALIGN_LEFT) || (align == ALIGN_TOP)) return left;
	if ((align == ALIGN_RIGHT) || (align == ALIGN_BOTTOM)) return right;
	return center;
}
