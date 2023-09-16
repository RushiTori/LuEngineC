#include "CollideBox.h"

enum CollideBoxErrCodes {
	CollideBoxIsNullptr = 0,
	CollideBoxAllocFail,
	CollideBoxVerticesIsNullptr,
	CollideBoxShapeIsUnknown,
	CollideBoxErrCount,
};

static const string CollideBoxErrorMessages[CollideBoxErrCount] = {
	"CollideBox Passed Is Nullptr",
	"CollideBox Allocation (malloc/calloc) Failed",
	"CollideBox Vertices Is Nullptr",
	"CollideBox Shape Is Unknown",
};

#define PRINT_COLLIDEBOX_ERROR(collideErr) \
	fprintf(stderr, "CollideBox Error %d : %s\n", collideErr, CollideBoxErrorMessages[collideErr])

CollideBox CollideBoxCreateLine(Vector2 posA, Vector2 posB) {
	CollideBox temp = EmptyCollideBox;
	temp.shape = BOX_LINE;

	temp.center = Vector2Scale(Vector2Add(posA, posB), 0.5f);
	temp.vertexCount = 2;

	temp.vertices = calloc(temp.vertexCount, sizeof(Vector2));
	if (!temp.vertices) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxAllocFail);
		return temp;
	}
	temp.vertices[0] = Vector2Subtract(posA, temp.center);
	temp.vertices[1] = Vector2Subtract(posB, temp.center);

	return temp;
}

CollideBox CollideBoxCreateCirc(Vector2 center, float r) {
	CollideBox temp = EmptyCollideBox;
	temp.shape = BOX_CIRCLE;

	temp.center = center;
	temp.vertexCount = 1;

	temp.vertices = calloc(temp.vertexCount, sizeof(Vector2));
	if (!temp.vertices) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxAllocFail);
		return temp;
	}
	temp.vertices[0] = (Vector2){r, 0};

	return temp;
}

CollideBox CollideBoxCreateRect(float x, float y, float w, float h) {
	CollideBox temp = EmptyCollideBox;
	temp.shape = BOX_RECT;

	temp.center.x = x + (w / 2);
	temp.center.y = y + (h / 2);

	temp.vertexCount = 4;

	temp.vertices = calloc(temp.vertexCount, sizeof(Vector2));
	if (!temp.vertices) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxAllocFail);
		return temp;
	}

	temp.vertices[0] = (Vector2){-w / 2, -h / 2};
	temp.vertices[1] = (Vector2){-w / 2, +h / 2};
	temp.vertices[2] = (Vector2){+w / 2, +h / 2};
	temp.vertices[3] = (Vector2){+w / 2, -h / 2};

	return temp;
}

CollideBox CollideBoxCreatePoly(Vector2 center, float r, uint sides, float angle) {
	CollideBox temp = EmptyCollideBox;
	switch (sides) {
		case 1:
			temp.shape = BOX_CIRCLE;
			break;
		case 2:
			temp.shape = BOX_LINE;
			break;
		case 3:
			temp.shape = BOX_TRIANGLE;
			break;
		default:
			temp.shape = BOX_CONVEX;
			break;
	}
	temp.center = center;
	temp.vertexCount = sides;

	temp.vertices = calloc(temp.vertexCount, sizeof(Vector2));
	if (!temp.vertices) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxAllocFail);
		return temp;
	}

	float angleDiff = TWO_PI / sides;

	for (uint i = 0; i < temp.vertexCount; i++) {
		float vertexAngle = angle - (angleDiff * i);
		temp.vertices[i] = (Vector2){cosf(vertexAngle) * r, sinf(vertexAngle) * r};
	}

	return temp;
}

void CollideBoxDestroy(CollideBox* box) {
	if (!box) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxIsNullptr);
		return;
	}

	free(box->vertices);

	(*box) = EmptyCollideBox;
}

void CollideBoxSetPos(CollideBox* box, Vector2 pos) {
	if (!box) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxIsNullptr);
		return;
	}
	box->center = pos;
}

void CollideBoxMoveBy(CollideBox* box, Vector2 vel) {
	if (!box) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxIsNullptr);
		return;
	}
	box->center = Vector2Add(box->center, vel);
}

void CollideBoxSetAngle(CollideBox* box, float angle) {
	if (!box) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxIsNullptr);
		return;
	}

	if (box->shape == BOX_CIRCLE) return;

	if (box->shape == BOX_RECT) box->shape = BOX_CONVEX;

	if (!box->vertices || !box->vertexCount) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxVerticesIsNullptr);
		return;
	}

	float baseAngle = atan2f(box->vertices[0].y, box->vertices[0].x);
	float rotateAmount = angle - baseAngle;

	for (uint i = 0; i < box->vertexCount; i++) {
		box->vertices[i] = Vector2Rotate(box->vertices[i], rotateAmount);
	}
}

void CollideBoxRotate(CollideBox* box, float rotateAmount) {
	if (!box) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxIsNullptr);
		return;
	}

	if (box->shape == BOX_CIRCLE) return;

	if (box->shape == BOX_RECT) box->shape = BOX_CONVEX;

	if (!box->vertices || !box->vertexCount) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxVerticesIsNullptr);
		return;
	}

	for (uint i = 0; i < box->vertexCount; i++) {
		box->vertices[i] = Vector2Rotate(box->vertices[i], rotateAmount);
	}
}

bool CheckCollisions(const CollideBox* boxA, const CollideBox* boxB) {
	if (!boxA || !boxB) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxIsNullptr);
		return false;
	}

	if ((!boxA->vertices || !boxB->vertices) || (!boxA->vertexCount || !boxB->vertexCount)) {
		PRINT_COLLIDEBOX_ERROR(CollideBoxVerticesIsNullptr);
		return false;
	}

	bool collision = false;

	uchar lhsOP = boxA->shape;
	uchar rhsOP = boxB->shape;
	if (boxA->shape > boxB->shape) return CheckCollisions(boxB, boxA);
	uchar opCode = (boxA->shape * 10) + boxB->shape;

	switch (opCode) {
		default:
			break;
		case 00:  // CircleToCircle
		{
			float rA = boxA->vertices[0].x;
			float rB = boxB->vertices[0].x;
			Vector2 cA = boxA->center;
			Vector2 cB = boxB->center;
			collision = CheckCollisionCircles(cA, rA, cB, rB);
		} break;
		case 01:  // CircleToLine
		{
			float rA = boxA->vertices[0].x;
			Vector2 cA = boxA->center;
			Vector2 pA = Vector2Add(boxB->center, boxB->vertices[0]);
			Vector2 pB = Vector2Add(boxB->center, boxB->vertices[1]);
			collision = CheckCollisionLineCircle(pA, pB, cA, rA);
		} break;
		case 02:  // CircleToTriangle
		{
			float rA = boxA->vertices[0].x;
			Vector2 cA = Vector2Subtract(boxA->center, boxB->center);

			Vector2* tVert = boxB->vertices;

			for (uint i = 0; i < 3; i++) {
				collision = CheckCollisionLineCircle(tVert[i], tVert[(i + 1) % 3], cA, rA);
				if (collision) break;
			}
			if (collision) break;
			collision = CheckCollisionPointTriangle(cA, tVert[0], tVert[1], tVert[2]);
		} break;
		case 03:  // CircleToRect
		{
			float rAA = boxA->vertices[0].x;
			Vector2 cA = Vector2Subtract(boxA->center, boxB->center);

			Vector2 rAB = boxB->vertices[0];
			Vector2 rCB = boxB->vertices[2];

			Vector2 ConRect = Vector2Clamp(cA, rAB, rCB);
			collision = CheckCollisionPointCircle(ConRect, boxA->center, rAA);
		} break;
		case 04:  // CircleToPoly
		{
			float rA = boxA->vertices[0].x;
			Vector2 cA = Vector2Subtract(boxA->center, boxB->center);
			for (uint i = 0; i < boxB->vertexCount; i++) {
				Vector2 pA = boxB->vertices[i];
				Vector2 pB = boxB->vertices[(i + 1) % boxB->vertexCount];
				collision = CheckCollisionLineCircle(pA, pB, cA, rA);
				if (collision) break;
			}
			if (collision) break;
			collision = CheckCollisionPointPolygon(cA, boxB->vertices, boxB->vertexCount);

		} break;
		case 11:  // LineToLine
		{
			Vector2 pAA = Vector2Add(boxA->center, boxA->vertices[0]);
			Vector2 pBA = Vector2Add(boxA->center, boxA->vertices[1]);

			Vector2 pAB = Vector2Add(boxB->center, boxB->vertices[0]);
			Vector2 pBB = Vector2Add(boxB->center, boxB->vertices[1]);

			collision = CheckCollisionLines(pAA, pBA, pAB, pBB, NULL);
		} break;
		case 12:  // LineToTriangle
		{
			Vector2 pA = Vector2Add(boxA->center, boxA->vertices[0]);
			Vector2 pB = Vector2Add(boxA->center, boxA->vertices[1]);

			Vector2 tA = Vector2Add(boxB->center, boxB->vertices[0]);
			Vector2 tB = Vector2Add(boxB->center, boxB->vertices[1]);
			Vector2 tC = Vector2Add(boxB->center, boxB->vertices[2]);

			collision = CheckCollisionPointTriangle(pA, tA, tB, tC);
			if (collision) break;

			collision = CheckCollisionPointTriangle(pB, tA, tB, tC);
			if (collision) break;

			collision = CheckCollisionLines(pA, pB, tA, tB, NULL);
			if (collision) break;

			collision = CheckCollisionLines(pA, pB, tA, tC, NULL);
			if (collision) break;

			collision = CheckCollisionLines(pA, pB, tC, tB, NULL);

		} break;
		case 13:  // LineToRect
		{
			Vector2 pA = Vector2Add(boxA->center, boxA->vertices[0]);
			Vector2 pB = Vector2Add(boxA->center, boxA->vertices[1]);

			Vector2 rA = Vector2Add(boxB->center, boxB->vertices[0]);
			Vector2 rB = Vector2Add(boxB->center, boxB->vertices[1]);
			Vector2 rC = Vector2Add(boxB->center, boxB->vertices[2]);
			Vector2 rD = Vector2Add(boxB->center, boxB->vertices[3]);

			Rectangle rec = (Rectangle){rA.x, rA.y, rC.x - rA.x, rC.y - rA.y};

			collision = CheckCollisionPointRec(pA, rec);
			if (collision) break;

			collision = CheckCollisionPointRec(pB, rec);
			if (collision) break;

			collision = CheckCollisionLines(pA, pB, rA, rB, NULL);
			if (collision) break;

			collision = CheckCollisionLines(pA, pB, rB, rC, NULL);
			if (collision) break;

			collision = CheckCollisionLines(pA, pB, rC, rD, NULL);
			if (collision) break;

			collision = CheckCollisionLines(pA, pB, rD, rA, NULL);
		} break;
		case 14:  // LineToPoly
		{
			Vector2 pA = Vector2Add(boxA->center, boxA->vertices[0]);
			Vector2 pB = Vector2Add(boxA->center, boxA->vertices[1]);

			pA = Vector2Subtract(pA, boxB->center);
			pB = Vector2Subtract(pB, boxB->center);

			for (uint i = 0; i < boxB->vertexCount; i++) {
				Vector2 tempA = boxB->vertices[i];
				Vector2 tempB = boxB->vertices[(i + 1) % boxB->vertexCount];
				collision = CheckCollisionLines(pA, pB, tempA, tempB, NULL);
				if (collision) break;
			}
			if (collision) break;

			collision = CheckCollisionPointPolygon(pA, boxB->vertices, boxB->vertexCount);
			if (collision) break;
			collision = CheckCollisionPointPolygon(pB, boxB->vertices, boxB->vertexCount);
		} break;
		case 22:  // TriangleToTriangle
		{
			Vector2 pVert[3];
			pVert[0] = Vector2Add(boxA->center, boxA->vertices[0]);
			pVert[1] = Vector2Add(boxA->center, boxA->vertices[1]);
			pVert[2] = Vector2Add(boxA->center, boxA->vertices[2]);

			Vector2 tVert[3];
			tVert[0] = Vector2Add(boxB->center, boxB->vertices[0]);
			tVert[1] = Vector2Add(boxB->center, boxB->vertices[1]);
			tVert[2] = Vector2Add(boxB->center, boxB->vertices[2]);

			for (uint i = 0; i < 3; i++) {
				collision = CheckCollisionPointTriangle(pVert[i], tVert[0], tVert[1], tVert[2]);
				if (collision) break;
				collision = CheckCollisionPointTriangle(tVert[i], pVert[0], pVert[1], pVert[2]);
				if (collision) break;
				for (uint j = 0; j < 3; j++) {
					collision = CheckCollisionLines(pVert[i], pVert[(i + 1) % 3], tVert[j], tVert[(j + 1) % 3], NULL);
					if (collision) break;
				}
				if (collision) break;
			}

		} break;
		case 23:  // TriangleToRect
		{
			Vector2 pVert[3];
			pVert[0] = Vector2Add(boxA->center, boxA->vertices[0]);
			pVert[1] = Vector2Add(boxA->center, boxA->vertices[1]);
			pVert[2] = Vector2Add(boxA->center, boxA->vertices[2]);

			Vector2 rVert[4];
			rVert[0] = Vector2Add(boxB->center, boxB->vertices[0]);
			rVert[1] = Vector2Add(boxB->center, boxB->vertices[1]);
			rVert[2] = Vector2Add(boxB->center, boxB->vertices[2]);
			rVert[3] = Vector2Add(boxB->center, boxB->vertices[3]);

			for (uint i = 0; i < 4; i++) {
				collision = CheckCollisionPointTriangle(rVert[i], pVert[0], pVert[1], pVert[2]);
				if (collision) break;
			}
			if (collision) break;

			Rectangle rec = (Rectangle){rVert[0].x, rVert[0].y, rVert[2].x - rVert[0].x, rVert[2].y - rVert[0].y};
			for (uint i = 0; i < 3; i++) {
				collision = CheckCollisionPointRec(pVert[i], rec);
				if (collision) break;
				for (uint j = 0; j < 4; j++) {
					collision = CheckCollisionLines(pVert[i], pVert[(i + 1) % 3], rVert[j], rVert[(j + 1) % 4], NULL);
					if (collision) break;
				}
				if (collision) break;
			}

		} break;
		case 24:  // TriangleToPoly
		{
			Vector2 pVert[3];
			pVert[0] = Vector2Add(boxA->center, boxA->vertices[0]);
			pVert[1] = Vector2Add(boxA->center, boxA->vertices[1]);
			pVert[2] = Vector2Add(boxA->center, boxA->vertices[2]);

			pVert[0] = Vector2Subtract(pVert[0], boxB->center);
			pVert[1] = Vector2Subtract(pVert[1], boxB->center);
			pVert[2] = Vector2Subtract(pVert[2], boxB->center);

			for (uint i = 0; i < boxB->vertexCount; i++) {
				Vector2 tempA = boxB->vertices[i];
				collision = CheckCollisionPointTriangle(tempA, pVert[0], pVert[1], pVert[2]);
				if (collision) break;

				Vector2 tempB = boxB->vertices[(i + 1) % boxB->vertexCount];
				for (uint j = 0; j < 3; j++) {
					collision = CheckCollisionLines(pVert[j], pVert[(j + 1) % 3], tempA, tempB, NULL);
					if (collision) break;
				}
				if (collision) break;
			}
			if (collision) break;

			for (uint i = 0; i < 3; i++) {
				collision = CheckCollisionPointPolygon(pVert[i], boxB->vertices, boxB->vertexCount);
				if (collision) break;
			}
		} break;
		case 33:  // RectToRect
		{
			Vector2 rAA = Vector2Add(boxA->center, boxA->vertices[0]);
			Vector2 rCA = Vector2Add(boxA->center, boxA->vertices[2]);

			Vector2 rAB = Vector2Add(boxB->center, boxB->vertices[0]);
			Vector2 rCB = Vector2Add(boxB->center, boxB->vertices[2]);
			if ((rAA.x > rCB.x || rCA.x < rAB.x) || (rAA.y > rCB.y || rCA.y < rAB.y)) {
				collision = false;
			} else {
				collision = true;
			}

		} break;
		case 34:  // RectToPoly
		{
			Vector2 rVert[4];
			rVert[0] = Vector2Add(boxA->center, boxA->vertices[0]);
			rVert[1] = Vector2Add(boxA->center, boxA->vertices[1]);
			rVert[2] = Vector2Add(boxA->center, boxA->vertices[2]);
			rVert[3] = Vector2Add(boxA->center, boxA->vertices[3]);

			rVert[0] = Vector2Subtract(rVert[0], boxB->center);
			rVert[1] = Vector2Subtract(rVert[1], boxB->center);
			rVert[2] = Vector2Subtract(rVert[2], boxB->center);
			rVert[3] = Vector2Subtract(rVert[3], boxB->center);

			Rectangle rec = (Rectangle){rVert[0].x, rVert[0].y, rVert[2].x - rVert[0].x, rVert[2].y - rVert[0].y};

			for (uint i = 0; i < boxB->vertexCount; i++) {
				Vector2 tempA = boxB->vertices[i];
				collision = CheckCollisionPointRec(tempA, rec);
				if (collision) break;
			}
			if (collision) break;

			for (uint i = 0; i < 4; i++) {
				collision = CheckCollisionPointPolygon(rVert[i], boxB->vertices, boxB->vertexCount);
				if (collision) break;

				for (uint j = 0; j < boxB->vertexCount; j++) {
					collision = CheckCollisionLines(rVert[i], rVert[(i + 1) % 4], boxB->vertices[j],
													boxB->vertices[(j + 1) % boxB->vertexCount], NULL);
					if (collision) break;
				}

				if (collision) break;
			}

		} break;
		case 44:  // PolyToPoly
		{
			for (uint i = 0; i < boxA->vertexCount; i++) {
				Vector2 temp = Vector2Add(boxA->center, boxA->vertices[i]);
				temp = Vector2Subtract(temp, boxB->center);
				collision = CheckCollisionPointPolygon(temp, boxB->vertices, boxB->vertexCount);
				if (collision) break;
			}

			if (collision) break;

			for (uint i = 0; i < boxB->vertexCount; i++) {
				Vector2 tempAA = Vector2Add(boxB->center, boxB->vertices[i]);
				tempAA = Vector2Subtract(tempAA, boxB->center);
				collision = CheckCollisionPointPolygon(tempAA, boxA->vertices, boxA->vertexCount);
				if (collision) break;

				Vector2 tempBA = Vector2Add(boxB->center, boxB->vertices[(i + 1) % boxB->vertexCount]);
				tempBA = Vector2Subtract(tempBA, boxB->center);
				for (uint j = 0; j < boxA->vertexCount; j++) {
					Vector2 tempAB = Vector2Add(boxA->center, boxA->vertices[j]);
					Vector2 tempBB = Vector2Add(boxA->center, boxA->vertices[(j + 1) % boxA->vertexCount]);
					collision = CheckCollisionLines(tempAA, tempBA, tempAB, tempBB, NULL);
					if (collision) break;
				}
				if (collision) break;
			}

		} break;
	}

	return collision;
}

bool CheckCollisionLineCircle(Vector2 pA, Vector2 pB, Vector2 center, float radius) {
	if (CheckCollisionPointCircle(pA, center, radius)) return true;
	if (CheckCollisionPointCircle(pB, center, radius)) return true;

	Vector2 AtoB = Vector2Subtract(pB, pA);
	Vector2 AtoC = Vector2Subtract(center, pA);
	float maxMag = Vector2Length(AtoB);
	AtoB = Vector2Normalize(AtoB);
	float scalarMag = Vector2DotProduct(AtoB, AtoC);

	if ((scalarMag <= 0) || (scalarMag >= maxMag)) return false;
	Vector2 ConAB = Vector2Add(pA, Vector2Scale(AtoB, scalarMag));
	return CheckCollisionPointCircle(ConAB, center, radius);
}

bool CheckCollisionPointPolygon(Vector2 point, Vector2* vertices, uint vertexCount) {
	int sideSign = 0;
	bool sameSide = true;
	for (uint i = 0; i < vertexCount; i++) {
		Vector2 pA = vertices[i];
		Vector2 pB = vertices[(i + 1) % vertexCount];
		Vector2 AtoB = Vector2Subtract(pB, pA);
		int side = signbit(((pB.x - pA.x) * (point.y - pA.y)) - ((pB.y - pA.y) * (point.x - pA.x)));
		if (i == 0) {
			sideSign = side;
		} else if (side != sideSign) {
			sameSide = false;
		}
	}
	return sameSide;
}
