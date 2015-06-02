#ifndef COH_CAMERA_H
#define COH_CAMERA_H

#include "math/Vec3.h"
#include "render/RenderObject.h"
#include "gl/Viewport.h"
#include "MatrixManager.h"

typedef struct Camera_s Camera;


typedef struct Camera_s {
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;

	//projection information
	scalar fov;
	scalar zNear;
	scalar zFar;

	//viewport object pointer
	Viewport* viewportObject;

	//void pointer to a parent game object
	RenderObject* parentObject;

} Camera;

typedef struct CameraManager_s {
	Camera*(* new)(Vec3* pos, Vec3* rot, Vec3* scl);

	void(* setPositionXYZ)(Camera* camera, scalar x, scalar y, scalar z);
	void(* addPositionXYZ)(Camera* camera, scalar x, scalar y, scalar z);
	void(* setPositionVec)(Camera* camera, Vec3* newPosition);
	void(* addPositionVec)(Camera* camera, Vec3* displacement);

	void(* setRotationXYZ)(Camera* camera, scalar x, scalar y, scalar z);
	void(* addRotationXYZ)(Camera* camera, scalar x, scalar y, scalar z);
	void(* setRotationVec)(Camera* camera, Vec3* rotation);
	void(* addRotationVec)(Camera* camera, Vec3* rotation);

	void(* setScaleXYZ)(Camera* camera, scalar x, scalar y, scalar z);
	void(* addScaleXYZ)(Camera* camera, scalar x, scalar y, scalar z);
	void(* setScaleVec)(Camera* camera, Vec3* scale);
	void(* addScaleVec)(Camera* camera, Vec3* scale);

	//sets the viewport
	void(* setViewportObject)(Camera* camera, Viewport* viewportObject);

	//sets the parent renderObject
	void(* setParentRenderObject)(Camera* camera, RenderObject* renderObject);

	//allows setting of projection information
	void(* setProjectionInfo)(scalar fov, scalar zNear, scalar zFar);

	//bind function
	void(* bind)(Camera* camera, MatrixManager* matrixMan);

	void(* unbind)();

} CameraManager;

#endif
