#ifndef COLLIDE_BOX_H
#define COLLIDE_BOX_H

#include "EngineUtils.h"

typedef enum CollideBoxShape {
	BOX_CIRCLE = 0,
	BOX_LINE = 1,
	BOX_TRIANGLE = 2,
	BOX_RECT = 3,
	BOX_CONVEX = 4,
	BOX_NONE = 5
} CollideBoxShape;

typedef struct CollideBox {
	CollideBoxShape shape;
	Vector2 center;
	Vector2* vertices;
	uint vertexCount;
} CollideBox;

#define EmptyCollideBox \
	(CollideBox) { BOX_CIRCLE, (Vector2){0, 0}, NULL, 0 }

CollideBox CollideBoxCreateLine(Vector2 posA, Vector2 posB);
CollideBox CollideBoxCreateCirc(Vector2 center, float r);
CollideBox CollideBoxCreateRect(float x, float y, float w, float h);
CollideBox CollideBoxCreatePoly(Vector2 center, float r, uint sides, float angle);

void CollideBoxDestroy(CollideBox* box);

void CollideBoxSetPos(CollideBox* box, Vector2 pos);
void CollideBoxMoveBy(CollideBox* box, Vector2 vel);
void CollideBoxSetAngle(CollideBox* box, float angle);
void CollideBoxRotate(CollideBox* box, float rotateAmount);

bool CheckCollisions(const CollideBox* boxA, const CollideBox* boxB);

bool CheckCollisionLineCircle(Vector2 pA, Vector2 pB, Vector2 center, float radius);
bool CheckCollisionPointPolygon(Vector2 point, Vector2* vertices, uint vertexCount);

#endif	// COLLIDE_BOX_H
