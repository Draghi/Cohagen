/*
 * Camera.c
 */

#include "Camera.h"

static const Vec3 xAxis = {1, 0, 0};
static const Vec3 yAxis = {0, 1, 0};
static const Vec3 zAxis = {0, 0, 1};

static Camera* new(Vec3* position, Vec3* rotation, Vec3* scale){
	Camera* camera = malloc(sizeof(Camera));

	if (position!=NULL) {
		camera->position = *position;
	} else {
		camera->position = manVec3.create(NULL, 0,0,0);
	}

	if (rotation!=NULL) {
		camera->rotation = *rotation;
	} else {
		camera->rotation = manVec3.create(NULL, 0,0,0);
	}

	if (scale!=NULL) {
		camera->scale = *scale;
	} else {
		camera->scale = manVec3.create(NULL, 1, 1, 1);
	}

	return camera;
}

static void bind(Camera* camera, MatrixManager* manMat){
	manMatMan.setMode(manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(manMat, camera->fov, (camera->viewport->width/camera->viewport->height), camera->zNear, camera->zFar);
	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.push(manMat);
	manMatMan.scale(manMat, camera->scale);
	manMatMan.rotate(manMat, camera->rotation.x, xAxis);
	manMatMan.rotate(manMat, camera->rotation.y, yAxis);
	manMatMan.rotate(manMat, camera->rotation.z, zAxis);
	manMatMan.translate(manMat, camera->position);
	if(camera->parentObject != NULL) {
		manMatMan.scale(manMat, *camera->parentObject->scale);
		manMatMan.rotate(manMat, camera->parentObject->rotation->x, xAxis);
		manMatMan.rotate(manMat, camera->parentObject->rotation->y, yAxis);
		manMatMan.rotate(manMat, camera->parentObject->rotation->z, zAxis);
		manMatMan.translate(manMat, *camera->parentObject->position);
	}
}

static void unbind(Camera* camera, MatrixManager* manMat){
	manMatMan.setMode(MATRIX_MODE_PROJECTION);
	manMatMan.pop(manMat);
	manMatMan.setMode(MATRIX_MODE_VIEW);
	manMatMan.pop(manMat);
}

static void setPositionXYZ(Camera* camera, scalar x, scalar y, scalar z){
	camera->position.x = x;
	camera->position.y = y;
	camera->position.z = z;
}

static void addPositionXYZ(Camera* camera, scalar x, scalar y, scalar z){
	camera->position.x += x;
	camera->position.y += y;
	camera->position.z += z;
}

static void setPositionVec(Camera* camera, Vec3* newPosition){
	setPositionXYZ(camera, newPosition->x, newPosition->y, newPosition->z);
}

static void addPositionVec(Camera* camera, Vec3* displacement){
	addPositionXYZ(camera, displacement->x, displacement->y, displacement->z);
}

static void setRotationXYZ(Camera* camera, scalar x, scalar y, scalar z){
	camera->rotation.x = x;
	camera->rotation.y = y;
	camera->rotation.z = z;
}

static void addRotationXYZ(Camera* camera, scalar x, scalar y, scalar z){
	camera->rotation.x += x;
	camera->rotation.y += y;
	camera->rotation.z += z;
}

static void setRotationVec(Camera* camera, Vec3* rotation){
	setRotationXYZ(camera, rotation->x, rotation->y, rotation->z);
}

static void addRotationVec(Camera* camera, Vec3* rotation){
	addRotationXYZ(camera, rotation->x, rotation->y, rotation->z);
}

static void setScaleXYZ(Camera* camera, scalar x, scalar y, scalar z){
	camera->scale.x = x;
	camera->scale.y = y;
	camera->scale.z = z;
}

static void addScaleXYZ(Camera* camera, scalar x, scalar y, scalar z){
	camera->scale.x += x;
	camera->scale.y += y;
	camera->scale.z += z;
}

static void setScaleVec(Camera* camera, Vec3* scale){
	setScaleXYZ(camera, scale->x, scale->y, scale->z);
}

static void addScaleVec(Camera* camera, Vec3* scale){
	addScaleXYZ(camera, scale->x, scale->y, scale->z);
}

//sets the viewport
static void setViewportObject(Camera* camera, Viewport* viewportObject){
	if(viewportObject != NULL){
		camera->viewport = viewportObject;
	} else {
		camera->viewport = manViewport.new(0.0, 0.0, 100, 100);
	}
}

//sets the parent renderObject
static void setParentRenderObject(Camera* camera, RenderObject* renderObject){
	if(renderObject != NULL){
		camera->parentObject = renderObject;
	} else {
		camera->parentObject = manRenderObj.new(NULL, NULL, NULL);
	}
}

//allows setting of projection information
static void setProjectionInfo(Camera* camera, scalar fov, scalar zNear, scalar zFar){

	camera->fov = fov;

	camera->zNear = zNear;

	camera->zFar = zFar;
}

static void delete(Camera *const camera){

}


const CameraManager manCamera = {new, bind, unbind, setPositionXYZ, addPositionXYZ, setPositionVec, addPositionVec, setRotationXYZ, addRotationXYZ, setRotationVec, addRotationVec, setScaleXYZ, addScaleXYZ, setScaleVec, addScaleVec, setViewportObject, setParentRenderObject, setProjectionInfo, delete};
