#ifndef COH_GAMELOOP_H
#define COH_GAMELOOP_H

#include <stdbool.h>
#include "glfw/Display.h"

typedef struct GameLoop_s GameLoop;

/** Called when the gameloop is being constructed **/
typedef void NewCallback(GameLoop* self);

/** Called then the window should be initialised. **/
typedef void InitWindowCallback(GameLoop* self);

/** Called when the context is created and opengl should be initialised.**/
typedef void InitOpenGLCallback(GameLoop* self);

/** Called after all other things are initialised. **/
typedef void InitMiscCallback(GameLoop* self);

/** Called everytime the world state should be updated **/
typedef void UpdateCallback(GameLoop* self, float tickDelta);

/** Called everytime the world should be rendered. **/
typedef void RenderCallback(GameLoop* self, float frameDelta);

/** Called when the main window of a gameloop is closed. **/
typedef void CloseCallback(GameLoop* self);

/** Called when the gameloop is being destroyed, being freed. **/
typedef void DestroyCallback(GameLoop* self);

/** Struct containing all the information for a given gameloop **/
typedef struct GameLoop_s {

	NewCallback* onNew;
	InitWindowCallback* onInitWindow;
	InitWindowCallback* onInitOpenGL;
	InitMiscCallback* onInitMisc;
	UpdateCallback* onUpdate;
	RenderCallback* onRender;
	CloseCallback* onClose;
	DestroyCallback* onDestroy;

	/** The primary window of the gameloop. Safe to swap to any non-null window.**/
	Window* primaryWindow;

	/** The internal accumulator used for calculating when to update the world. Not recommended to alter.**/
	double timeAccumulator;

	/** The target time in ms a frame should take to render. May be changed at runtime. **/
	double targetFrameTime;

	/** The target time in ms a tick should take to update. May be changed at runtime. **/
	double targetTickTime;

	/** FPS **/
	double frameStartTime;
	int frameCount;
	double fps;

	/** TPS **/
	double tickStartTime;
	int tickCount;
	double tps;

	/** An anchor point for a loop's extra data. Must be manually freed if used.**/
	void* extraData;
} GameLoop;

/** The game loop manager. Responsible for the creation, use and clean-up of gameloops.**/
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
	GameLoop*(* new)(NewCallback* onNew, InitWindowCallback* onInitWindow, InitOpenGLCallback* onInitOpenGL, InitMiscCallback* onInitMisc, UpdateCallback* onUpdate, RenderCallback* onRender, CloseCallback* onClose, DestroyCallback* onDestroy, double targetTickTime, double targetFrameTime);

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
