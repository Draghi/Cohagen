#ifndef COH_GAMELOOP_H
#define COH_GAMELOOP_H

#include <stdbool.h>
#include "glfw/Display.h"

typedef struct GameLoop_s GameLoop;

typedef void NewCallback(GameLoop* self);
typedef void InitWindowCallback(GameLoop* self);
typedef void InitOpenGLCallback(GameLoop* self);
typedef void InitMiscCallback(GameLoop* self);
typedef void UpdateCallback(GameLoop* self, float tickDelta);
typedef void RenderCallback(GameLoop* self, float frameDelta);
typedef void CloseCallback(GameLoop* self);
typedef void DestroyCallback(GameLoop* self);

typedef struct GameLoop_s {
	NewCallback*   onNew;
	InitWindowCallback*   onInitWindow;
	InitWindowCallback*   onInitOpenGL;
	InitMiscCallback*     onInitMisc;
	UpdateCallback* onUpdate;
	RenderCallback* onRender;
	CloseCallback*  onClose;
	DestroyCallback*  onDestroy;

	Window* primaryWindow;

	double timeAccumulator;
	double targetFrameTime;
	double targetTickTime;

	void* extraData;
} GameLoop;

typedef struct GameLoopManager_s {
	/**
	 * Creates a new gameloop.
	 *
	 * @param onNew The function to call when the gameloop is created.
	 * @param onInit The function to call when the gameloop is being started.
	 * @param onUpdate The function to call when the gameloop is updating.
	 * @param onRender The function to call when the gameloop is rendering.
	 * @param onClose The function to call when the gameloop is closing.
	 * @param onDestroy The function to call when the gameloop is being destroyed.
	 * @param targetFrameTime The ideal time it takes to render 1 frame (1/60ms for a 60hz target)
	 * @param targetTickTime The ideal time it takes to update 1 tick (1/120ms for a 120hz target)
	 *
	 * @return The newly constructed gameloop.
	 */
	GameLoop*(* new)(NewCallback* onNew, InitWindowCallback* onInitWindow, InitOpenGLCallback* onInitOpenGL, InitMiscCallback* onInitMisc, UpdateCallback* onUpdate, RenderCallback* onRender, CloseCallback* onClose, DestroyCallback* onDestroy, double targetFrameTime, double targetTickTime);

	/**
	 * Causes the current thread to enter the given game loop.
	 *
	 * @param gameloop The gameloop to start.
	 */
	void(* enterGameLoop)(GameLoop* gameloop);

	/**
	 * Destroys the given game loop, freeing it from memory.
	 *
	 * @param gameloop
	 *
	 * @remarks Do not call while currently running in a loop. Causes undefined behaviour.
	 */
	void(* delete)(GameLoop* gameloop);
} GameLoopManager;

extern const GameLoopManager manGameLoop;

#endif
