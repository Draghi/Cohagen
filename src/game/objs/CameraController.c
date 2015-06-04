#include "CameraController.h"

typedef struct CameraControllerData_s {
	Camera* camera;
	scalar rate;
	scalar maxRate;
	scalar minRate;
	int keyForward;
	int keyBackward;
	int keyLeft;
	int keyRight;
	int keyUp;
	int keyDown;
	int keyFaster;
	int keySlower;

} CameraControllerData;

static void doCameraControl(Camera* camera, Window* window, scalar rate, int keyForward, int keyBackward, int keyLeft, int keyRight, int keyUp, int keyDown, int keyFaster, int keySlower) {
	/* ************* *
	 * Mouse Control *
	 * ************* */
	manCamera.addRotationXYZ(camera, -manMouse.getAbsoluteDY(window)/100.0, manMouse.getAbsoluteDX(window)/100.0, 0);

	//Limit camera from doing flips.
	if (camera->rotation.x > 1.57079633)
		camera->rotation.x = 1.57079633;

	if (camera->rotation.x < -1.57079633)
		camera->rotation.x = -1.57079633;

	bool moveForward  = manKeyboard.isDown(window, keyForward);
	bool moveBackward = manKeyboard.isDown(window, keyBackward);
	bool moveLeft = manKeyboard.isDown(window, keyLeft);
	bool moveRight = manKeyboard.isDown(window, keyRight);
	bool moveUp = manKeyboard.isDown(window, keyUp);
	bool moveDown = manKeyboard.isDown(window, keyDown);

	/* ************************* *
	 * Forward and Back movement *
	 * ************************* */
	if ((moveForward) || (moveBackward)) {
		//Calculate the "forward" direction.
		Vec3 xAxis = {1, 0, 0};
		Vec3 yAxis = {0, 1, 0};
		Vec3 zAxis = {0, 0, 1};

		Mat4 rot = manMat4.createLeading(NULL, 1);
		rot = manMat4.affRotate(&rot, camera->rotation.z, &zAxis);
		rot = manMat4.affRotate(&rot, camera->rotation.y, &yAxis);
		rot = manMat4.affRotate(&rot, camera->rotation.x, &xAxis);

		Vec4 forward = {0,0,1,0};
		forward = manMat4.postMulVec4(&rot, &forward);

		Vec3 movement = manVec3.create(NULL, forward.x, forward.y, -forward.z);
		if (manVec3.magnitude(&movement)>0)
			movement =manVec3.normalize(&movement);

		if (moveForward) {
			Vec3 forwardMovement = manVec3.preMulScalar(rate, &movement);
			manCamera.addPositionVec(camera, &forwardMovement);
		}

		if (moveBackward) {
			Vec3 backwardMovement = manVec3.preMulScalar(-rate, &movement);
			manCamera.addPositionVec(camera, &backwardMovement);
		}
	}
	/* *********************** *
	 * Left and Right movement *
	 * *********************** */
	if ((moveLeft) || (moveRight)) {
		Vec3 movement = manVec3.create(NULL, cos(camera->rotation.y), 0, sin(camera->rotation.y));
		if (manVec3.magnitude(&movement)>0)
			movement = manVec3.normalize(&movement);

		if (moveRight) {
			Vec3 rightMovement = manVec3.preMulScalar(rate, &movement);
			manCamera.addPositionVec(camera, &rightMovement);
		}

		if (moveLeft) {
			Vec3 leftMovement = manVec3.preMulScalar(-rate, &movement);
			manCamera.addPositionVec(camera, &leftMovement);
		}
	}

	/* ******************** *
	 * Up and Down movement *
	 * ******************** */
	if ((moveUp) || (moveDown)) {
		Vec3 movement = manVec3.create(NULL, 0, 1, 0);
		if (manVec3.magnitude(&movement)>0)
			movement =manVec3.normalize(&movement);

		if (moveUp) {
			Vec3 upMovement = manVec3.preMulScalar(rate, &movement);
			manCamera.addPositionVec(camera, &upMovement);
		}

		if (moveDown) {
			Vec3 downMovement = manVec3.preMulScalar(-rate, &movement);
			manCamera.addPositionVec(camera, &downMovement);
		}
	}
}

static void onUpdate(GameObject* self, float tickDelta) {
	CameraControllerData* data = self->parent;

	if (manKeyboard.isDown(self->window, KEY_Q)) {
		data->rate -= data->rate*tickDelta;
		if (data->rate<data->minRate)
			data->rate = data->minRate;
	}

	if (manKeyboard.isDown(self->window, KEY_E)) {
		data->rate += data->rate*tickDelta;
		if (data->rate>data->maxRate)
			data->rate = data->maxRate;
	}

	doCameraControl(data->camera, self->window, data->rate*tickDelta, data->keyForward, data->keyBackward, data->keyLeft, data->keyRight, data->keyUp, data->keyDown, data->keyFaster, data->keySlower);
}

GameObject* newCameraController(Camera* camera, Window* window, scalar movementSpeed, scalar movementSpeedMax, scalar movementSpeedMin, int keyForward, int keyBackward, int keyLeft, int keyRight, int keyUp, int keyDown, int keyFaster, int keySlower) {
	CameraControllerData* data = malloc(sizeof(CameraControllerData));

	data->camera = camera;
	data->maxRate = movementSpeedMax;
	data->minRate = movementSpeedMin;
	data->rate = movementSpeed;
	data->keyForward = keyForward;
	data->keyBackward = keyBackward;
	data->keyLeft = keyLeft;
	data->keyRight = keyRight;
	data->keyUp = keyUp;
	data->keyDown = keyDown;
	data->keyFaster = keyFaster;
	data->keySlower = keySlower;

	return manGameObj.new("CameraController", data, false, false, onUpdate, NULL, NULL, NULL, window);
}
