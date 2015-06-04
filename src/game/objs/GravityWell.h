#ifndef GAME_OBJS_GRAVITYWELL_H_
#define GAME_OBJS_GRAVITYWELL_H_

#include "engine/GameObjectRegistry.h"
#include "glfw/Display.h"

void prepareGrav(Shader* shader);
GameObject* newGrav(GameObjectRegist* regist, Window* window, Vec3 pos, Vec3 rot, Vec3 scl);

#endif
