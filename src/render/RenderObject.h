#ifndef COH_RENDEROBJECT_H
#define COH_RENDEROBJECT_H

#include "math/Vec3.h"
#include "gl/VAO.h"
#include "gl/Textures.h"
#include "util/DynamicArray.h"

typedef struct RenderObject_s {
	Vec3* position;
	Vec3* rotation;
	Vec3* scale;

	VAO* model;
	DynamicArray* textures;
} RenderObject;

typedef struct RenderObjectManager_s {
	RenderObject*(* new)(Vec3 *position, Vec3* rotation, Vec3* scale);
	void(* setModel)(RenderObject* renderObject, VAO* vao);
	void(* addTexture)(RenderObject* renderObject, Texture* texture);
	void(* delete)(RenderObject* renderObject);
} RenderObjectManager;

extern const RenderObjectManager manRenderObj;

#endif
