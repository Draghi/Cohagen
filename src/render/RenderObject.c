#include "RenderObject.h"

#include <assert.h>

static RenderObject* new(Vec3 *position, Vec3* rotation, Vec3* scale) {
	RenderObject* renderObject = malloc(sizeof(RenderObject));

	if (position!=NULL) {
		renderObject->position = position;
	} else {
		renderObject->position = malloc(sizeof(Vec3));
		manVec3.create(renderObject->position, 0,0,0);
	}

	if (rotation!=NULL) {
		renderObject->rotation = rotation;
	} else {
		renderObject->rotation = malloc(sizeof(Vec3));
		manVec3.create(renderObject->rotation, 0,0,0);
	}

	if (scale!=NULL) {
		renderObject->scale = scale;
	} else {
		renderObject->scale = malloc(sizeof(Vec3));
		manVec3.create(renderObject->scale, 1, 1, 1);
	}

	renderObject->model = NULL;
	renderObject->textures = manDynamicArray.new(1, sizeof(Texture*));

	return renderObject;
}

static void setModel(RenderObject* renderObject, VAO* model){
	renderObject->model = model;
}

static void addTexture(RenderObject* renderObject, Texture* texture){
	assert(texture!=NULL);

	manDynamicArray.append(renderObject->textures, texture);
}

static void delete(RenderObject* renderObject) {
	manDynamicArray.delete(renderObject->textures);
}

const RenderObjectManager manRenderObj = {new, setModel, addTexture, delete};
