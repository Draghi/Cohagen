#ifndef COH_CAMERA_H
#define COH_CAMERA_H

#include "math/Vec3.h"

typedef struct Camera_s {
	Vec3 pos;
	Vec3 rot;
	Vec3 scl;

	scalar fov;
	scalar vpXPos;
	scalar vpYPos;
	scalar vpWidth;
	scalar vpHeight;
	scalar zNear;
	scalar zFar;

	void* parentObject; //Implement when GameObjects are added.
} Camera;

typedef struct CameraManager_s {
	Camera*(* new)(Vec3* pos, Vec3* rot, Vec3* scl, scalar fov);
} CameraManager;

#endif
