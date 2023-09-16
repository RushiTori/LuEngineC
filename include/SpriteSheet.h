#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "Drawable.h"

typedef struct SpriteSheet {
	Texture* tex;
	Drawable* cells;
	uint width, height;
} SpriteSheet;

#define EmptySpriteSheet \
	(SpriteSheet) { NULL, NULL, 0, 0 }

SpriteSheet SpriteSheetCreate(const string sheetPath, uint cellW, uint cellH);
void SpriteSheetDestroy(SpriteSheet* sheet);
Drawable SpriteSheetGetTexture(const SpriteSheet* sheet, uint x, uint y);

#endif	// SPRITE_SHEET_H
