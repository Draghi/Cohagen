#ifndef COH_RENDEROBJECT_H
#define COH_RENDEROBJECT_H

#include "math/Vec3.h"
#include "gl/VAO.h"
#include "gl/Textures.h"
#include "util/DynamicArray.h"

typedef struct RenderObject_s {
	/** Pointer to the position to use. **/
	Vec3* position;
	/** Pointer to the rotation to use. **/
	Vec3* rotation;
	/** Pointer to the scale to use.**/
	Vec3* scale;

	/** Poiner to the model to use **/
	VAO* model;

	/** A dynamic array of textures. **/
	DynamicArray* textures;
} RenderObject;

typedef struct RenderObjectManager_s {
	/**
	 * Creates a new renderobject.
	 * @remark The value of the pointer is store (The render object will reference them)
	 * @param position The pointer to the position to use. NULL will allocate a new position.
	 * @param rotation The pointer to the rotation to use. NULL will allocate a new rotation.
	 * @param scale The pointer to the scale to use. NULL will allocate a new scale.
	 * @return A new RenderObject.
	 */
	RenderObject*(* new)(Vec3 *position, Vec3* rotation, Vec3* scale);

	/**
	 * Changes the VAO of the given render object.
	 * @param renderObject The RenderObject to modify.
	 * @param vao The new VAO to use.
	 */
	void(* setModel)(RenderObject* renderObject, VAO* vao);

	/**
	 * Adds a new texture to the Render Object.
	 * @param renderObject The RenderObject to modify.
	 * @param texture The texture to add.
	 * @remarks The order in which they are added, defines which slot they will use.
	 */
	void(* addTexture)(RenderObject* renderObject, Texture* texture);

	/**
	 * Deletes the renderobject.
	 * @remark Does not free position, rotation, scale or VAO. That is up to the caller.
	 * @param renderObject The RenderObject to delete.
	 */
	void(* delete)(RenderObject* renderObject);
} RenderObjectManager;

extern const RenderObjectManager manRenderObj;

#endif
