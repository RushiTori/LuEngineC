#include "Drawable.h"

enum DrawableErrCodes {
	DrawableIsNullptr = 0,
	DrawableAllocFail,
	DrawableVerticesIsNullptr,
	DrawableTexcoordsIsNullptr,
	DrawableErrCount,
};

static const string DrawableErrorMessages[DrawableErrCount] = {
	"Drawable Passed Is Nullptr",
	"Drawable Allocation (malloc/calloc) Failed",
	"Drawable Vertices Is Nullptr",
	"Drawable UV Is Nullptr",
};

#define PRINT_DRAWABLE_ERROR(drawErr) \
	fprintf(stderr, "Drawable Error %d : %s\n", drawErr, DrawableErrorMessages[drawErr])

Drawable DrawableCreateRect(float x, float y, float w, float h) {
	Drawable temp = EmptyDrawable;

	temp.center.x = x + (w / 2);
	temp.center.y = y + (h / 2);

	temp.vertexCount = 4;
	temp.vertices = calloc(temp.vertexCount, sizeof(Vector2));

	if (!temp.vertices) {
		PRINT_DRAWABLE_ERROR(DrawableAllocFail);
		return EmptyDrawable;
	}

	temp.vertices[0] = (Vector2){-w / 2, -h / 2};
	temp.vertices[1] = (Vector2){-w / 2, +h / 2};
	temp.vertices[2] = (Vector2){+w / 2, +h / 2};
	temp.vertices[3] = (Vector2){+w / 2, -h / 2};

	return temp;
}

Drawable DrawableCreatePoly(float x, float y, float r, uint sides, float angle) {
	Drawable temp = EmptyDrawable;
	temp.center = (Vector2){x, y};

	temp.vertexCount = sides;
	temp.vertices = calloc(temp.vertexCount, sizeof(Vector2));

	if (!temp.vertices) {
		PRINT_DRAWABLE_ERROR(DrawableAllocFail);
		return EmptyDrawable;
	}

	float angleDiff = TWO_PI / sides;

	for (uint i = 0; i < temp.vertexCount; i++) {
		float vertexAngle = angle - (angleDiff * i);
		temp.vertices[i] = (Vector2){cosf(vertexAngle) * r, sinf(vertexAngle) * r};
	}

	return temp;
}

Drawable DrawableCopy(const Drawable* drawable) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return EmptyDrawable;
	}

	if (!drawable->vertices || !drawable->vertexCount) {
		PRINT_DRAWABLE_ERROR(DrawableVerticesIsNullptr);
		return EmptyDrawable;
	}

	Drawable temp = *drawable;

	temp.vertices = calloc(temp.vertexCount, sizeof(Vector2));
	if (!temp.vertices) {
		PRINT_DRAWABLE_ERROR(DrawableAllocFail);
		return EmptyDrawable;
	}

	memcpy(temp.vertices, drawable->vertices, sizeof(Vector2) * temp.vertexCount);

	if (drawable->uv) {
		temp.uv = calloc(temp.vertexCount, sizeof(Vector2));
		if (!temp.uv) {
			PRINT_DRAWABLE_ERROR(DrawableAllocFail);
			free(temp.uv);
			return EmptyDrawable;
		}
		memcpy(temp.uv, drawable->uv, sizeof(Vector2) * temp.vertexCount);
	}

	return temp;
}

void DrawableDestroy(Drawable* drawable) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	free(drawable->vertices);
	free(drawable->uv);
	(*drawable) = EmptyDrawable;
}

void DrawableSetPos(Drawable* drawable, Vector2 pos) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	drawable->center = pos;
}

void DrawableMoveBy(Drawable* drawable, Vector2 amount) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	drawable->center = Vector2Add(drawable->center, amount);
}

void DrawableSetTextureRect(Drawable* drawable, Texture* tex, Rectangle source) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	if (!drawable->vertices) {
		PRINT_DRAWABLE_ERROR(DrawableVerticesIsNullptr);
		return;
	}

	if (!drawable->uv) {
		drawable->uv = calloc(4 + 1, sizeof(Vector2));
		if (!drawable->uv) {
			PRINT_DRAWABLE_ERROR(DrawableAllocFail);
			return;
		}
	}

	drawable->tex = tex;

	Vector2 uvMin = (Vector2){source.x / tex->width, source.y / tex->height};
	Vector2 uvMax = (Vector2){(source.x + source.width) / tex->width, (source.y + source.height) / tex->height};

	drawable->uv[0] = (Vector2){uvMin.x, uvMin.y};
	drawable->uv[1] = (Vector2){uvMin.x, uvMax.y};
	drawable->uv[2] = (Vector2){uvMax.x, uvMax.y};
	drawable->uv[3] = (Vector2){uvMax.x, uvMin.y};

	drawable->uvCenter = Vector2Scale(Vector2Add(uvMin, uvMax), 0.5f);
}

void DrawableSetTexturePoly(Drawable* drawable, Texture* tex, Rectangle source) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	if (!drawable->vertices) {
		PRINT_DRAWABLE_ERROR(DrawableVerticesIsNullptr);
		return;
	}

	if (!drawable->uv) {
		drawable->uv = calloc(drawable->vertexCount + 1, sizeof(Vector2));
		if (!drawable->uv) {
			PRINT_DRAWABLE_ERROR(DrawableAllocFail);
			return;
		}
	}

	drawable->tex = tex;

	Vector2 uvMin = (Vector2){source.x / tex->width, source.y / tex->height};
	Vector2 uvMax = (Vector2){(source.x + source.width) / tex->width, (source.y + source.height) / tex->height};

	for (uint i = 0; i < drawable->vertexCount; i++) {
		Vector2 texVertex = Vector2Normalize(drawable->vertices[i]);
		texVertex = Vector2Scale(texVertex, 0.5f);
		texVertex = Vector2AddValue(texVertex, 0.5f);
		texVertex = Vector2Clamp(texVertex, uvMin, uvMax);
		drawable->uv[i] = texVertex;
	}
}

void DrawableSetAngle(Drawable* drawable, float angle) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	if (!drawable->vertices) {
		PRINT_DRAWABLE_ERROR(DrawableVerticesIsNullptr);
		return;
	}

	float baseAngle = atan2f(drawable->vertices[0].y, drawable->vertices[0].x);
	float angleDiff = angle - baseAngle;

	DrawableRotate(drawable, angleDiff);
}

void DrawableRotate(Drawable* drawable, float rotateAmount) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	if (!drawable->vertices) {
		PRINT_DRAWABLE_ERROR(DrawableVerticesIsNullptr);
		return;
	}

	for (uint i = 0; i < drawable->vertexCount; i++) {
		drawable->vertices[i] = Vector2Rotate(drawable->vertices[i], rotateAmount);
	}
}

void DrawableDraw(const Drawable* drawable) {
	if (!drawable) {
		PRINT_DRAWABLE_ERROR(DrawableIsNullptr);
		return;
	}

	if (!drawable->vertices) {
		PRINT_DRAWABLE_ERROR(DrawableVerticesIsNullptr);
		return;
	}

	if (drawable->tex) {
		if (!drawable->uv) {
			PRINT_DRAWABLE_ERROR(DrawableTexcoordsIsNullptr);
			return;
		}
		DrawTexturePolyLU(drawable->tex, drawable->center, drawable->vertices, drawable->uvCenter, drawable->uv,
						  drawable->vertexCount, drawable->tint);
	} else {
		float radius = Vector2Length(drawable->vertices[0]);
		float rotation = atan2f(drawable->vertices[0].y, drawable->vertices[0].x);
		DrawPoly(drawable->center, drawable->vertexCount, radius, rotation * -RAD_TO_DEG, drawable->tint);
	}
}

void DrawTexturePolyLU(const Texture* tex, Vector2 center, const Vector2* vertices, Vector2 uvCenter, const Vector2* uv,
					   uint sides, Color tint) {
	if (sides < 3) return;

	rlSetTexture(tex->id);

	// Texturing is only supported on RL_QUADS
	rlBegin(RL_QUADS);

	rlColor4ub(tint.r, tint.g, tint.b, tint.a);

	for (uint i = 0; i < sides; i++) {
		uint j = (i + 1) % sides;

		rlTexCoord2f(uvCenter.x, uvCenter.y);
		rlVertex2f(center.x, center.y);

		rlTexCoord2f(uv[i].x, uv[i].y);
		rlVertex2f(vertices[i].x + center.x, vertices[i].y + center.y);

		rlTexCoord2f(uv[j].x, uv[j].y);
		rlVertex2f(vertices[j].x + center.x, vertices[j].y + center.y);

		rlTexCoord2f(uv[j].x, uv[j].y);
		rlVertex2f(vertices[j].x + center.x, vertices[j].y + center.y);
	}

	rlEnd();

	rlSetTexture(0);
}
