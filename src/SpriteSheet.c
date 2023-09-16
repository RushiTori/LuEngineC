#include "SpriteSheet.h"

enum SpriteSheetErrCodes {
	SpriteSheetIsNullptr = 0,
	SpriteSheetAllocFail,
	SpriteSheetBaseTextureErr,
	SpriteSheetOutOfBounds,
	SpriteSheetCellTextureErr,
	SpriteSheetErrCount,
};

static const string SpriteSheetErrorMessages[SpriteSheetErrCount] = {
	"SpriteSheet Passed Is Nullptr",
	"SpriteSheet Allocation (malloc/calloc) Failed",
	"SpriteSheet Could Not Create Base Texture",
	"SpriteSheet Index Out Of Bounds",
	"SpriteSheet Could Not Create Cell Texture",
};

#define PRINT_SPRITE_SHEET_ERROR(sheetErr) \
	fprintf(stderr, "SpriteSheet Error %d : %s\n", sheetErr, SpriteSheetErrorMessages[sheetErr])

SpriteSheet SpriteSheetCreate(const string sheetPath, uint cellW, uint cellH) {
	SpriteSheet temp = EmptySpriteSheet;

	temp.tex = malloc(sizeof(Texture));
	if (!temp.tex) {
		PRINT_SPRITE_SHEET_ERROR(SpriteSheetAllocFail);
		return EmptySpriteSheet;
	}

	*(temp.tex) = LoadTexture(sheetPath);
	if (!temp.tex->id) {
		PRINT_SPRITE_SHEET_ERROR(SpriteSheetBaseTextureErr);
		return EmptySpriteSheet;
	}

	temp.width = temp.tex->width / cellW;
	temp.height = temp.tex->height / cellH;

	temp.cells = calloc(temp.width * temp.height, sizeof(Drawable));
	if (!temp.cells) {
		PRINT_SPRITE_SHEET_ERROR(SpriteSheetAllocFail);
		SpriteSheetDestroy(&temp);
		return EmptySpriteSheet;
	}

	for (uint y = 0; y < temp.height; y++) {
		uint uvY = y * cellH;
		for (uint x = 0; x < temp.width; x++) {
			uint uvX = x * cellW;
			Rectangle uvSource = (Rectangle){uvX, uvY, cellW, cellH};
			uint idx = index1D(x, y, temp.width);

			temp.cells[idx] = DrawableCreateRect(0, 0, cellW, cellH);
			DrawableSetTextureRect(temp.cells + idx, temp.tex, uvSource);
		}
	}

	return temp;
}

void SpriteSheetDestroy(SpriteSheet* sheet) {
	if (!sheet) {
		PRINT_SPRITE_SHEET_ERROR(SpriteSheetIsNullptr);
	}

	if (sheet->tex) {
		UnloadTexture(*(sheet->tex));
		free(sheet->tex);
	}

	for (uint i = 0; i < sheet->width * sheet->height; i++) {
		DrawableDestroy(sheet->cells + i);
	}
	free(sheet->cells);

	*sheet = EmptySpriteSheet;
}

Drawable SpriteSheetGetTexture(const SpriteSheet* sheet, uint x, uint y) {
	if (!sheet) {
		PRINT_SPRITE_SHEET_ERROR(SpriteSheetIsNullptr);
		return EmptyDrawable;
	}

	if ((x >= sheet->width) || (y >= sheet->height)) {
		PRINT_SPRITE_SHEET_ERROR(SpriteSheetOutOfBounds);
		return EmptyDrawable;
	}

	return sheet->cells[index1D(x, y, sheet->width)];
}
