#ifndef COH_ASTEROID_H_
#define COH_ASTEROID_H_

#include "engine/GameObject.h"
#include "glfw/Display.h"

void prepareAsteroids(Shader* shader);
GameObject* newAsteroid(Window* window, Vec3 pos, Vec3 rot, Vec3 scl);

#endif
