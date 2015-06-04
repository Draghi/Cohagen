#ifndef COH_CAMERACONTROLLER_H
#define COH_CAMERACONTROLLER_H

#include "engine/GameObject.h"
#include "render/Camera.h"
#include "glfw/Display.h"

GameObject* newCameraController(Camera* camera, Window* window, scalar movementSpeed, scalar movementSpeedMax, scalar movementSpeedMin, int keyForward, int keyBackward, int keyLeft, int keyRight, int keyUp, int keyDown, int keyFaster, int keySlower);

#endif
