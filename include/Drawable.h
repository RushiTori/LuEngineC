#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "EngineUtils.h"

typedef enum DrawableLayer {
	LAYER_BACKGROUND = 0,
} DrawableLayer;

typedef struct Drawable {
	DrawableLayer layer;
	Texture* tex;
	Vector2 center;
	Vector2* vertices;
	Vector2 uvCenter;
	Vector2* uv;
	uint vertexCount;
	Color tint;
} Drawable;

#define EmptyDrawable \
	(Drawable) { LAYER_BACKGROUND, NULL, (Vector2){0, 0}, NULL, (Vector2){0, 0}, NULL, 0, WHITE }

Drawable DrawableCreateRect(float x, float y, float w, float h);
Drawable DrawableCreatePoly(float x, float y, float r, uint sides, float angle);
Drawable DrawableCopy(const Drawable* drawable);
void DrawableDestroy(Drawable* drawable);

void DrawableSetPos(Drawable* drawable, Vector2 pos);
void DrawableMoveBy(Drawable* drawable, Vector2 amount);
void DrawableSetTextureRect(Drawable* drawable, Texture* tex, Rectangle source);
void DrawableSetTexturePoly(Drawable* drawable, Texture* tex, Rectangle source);
void DrawableSetAngle(Drawable* drawable, float angle);
void DrawableRotate(Drawable* drawable, float rotateAmount);

void DrawableDraw(const Drawable* drawable);
void DrawTexturePolyLU(const Texture* tex, Vector2 center, const Vector2* vertices, Vector2 uvCenter, const Vector2* uv,
					   uint sides, Color tint);

#endif	// DRAWABLE_H
