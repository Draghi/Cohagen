#include "GameLoop.h"

#include <stdlib.h>

/*
 * Helper funcs
 */
static void doOnNew(GameLoop* gameloop) {
	if (gameloop->onNew != NULL)
		gameloop->onNew(gameloop);
}

static void doOnInitWindow(GameLoop* gameloop) {
	if (gameloop->onInitWindow != NULL)
		gameloop->onInitWindow(gameloop);
}

static void doOnInitOpenGL(GameLoop* gameloop) {
	if (gameloop->onInitOpenGL != NULL)
		gameloop->onInitOpenGL(gameloop);
}

static void doOnInitMisc(GameLoop* gameloop) {
	if (gameloop->onInitMisc != NULL)
		gameloop->onInitMisc(gameloop);
}

static void doOnRender(GameLoop* gameloop) {
	if (gameloop->onRender != NULL)
		gameloop->onRender(gameloop, gameloop->targetFrameTime);
}

static void doOnUpdate(GameLoop* gameloop) {
	if (gameloop->onUpdate!=NULL)
		gameloop->onUpdate(gameloop, gameloop->targetTickTime);
}

static void doOnClose(GameLoop* gameloop) {
	if (gameloop->onClose!=NULL)
		gameloop->onClose(gameloop);
}

static void doOnDestroy(GameLoop* gameloop) {
	if (gameloop->onDestroy!=NULL)
		gameloop->onDestroy(gameloop);
}

/*
 * Manager Functions
 */
static GameLoop* new(NewCallback* onNew, InitWindowCallback* onInitWindow, InitOpenGLCallback* onInitOpenGL, InitMiscCallback* onInitMisc, UpdateCallback* onUpdate, RenderCallback* onRender, CloseCallback* onClose, DestroyCallback* onDestroy, double targetTickTime, double targetFrameTime){
	GameLoop* gameloop = malloc(sizeof(GameLoop));

	gameloop->onNew = onNew;
	gameloop->onInitWindow = onInitWindow;
	gameloop->onInitOpenGL = onInitMisc;
	gameloop->onInitMisc = onInitOpenGL;
	gameloop->onUpdate = onUpdate;
	gameloop->onRender = onRender;
	gameloop->onClose = onClose;
	gameloop->onDestroy = onDestroy;

	gameloop->primaryWindow = manWin.new();

	gameloop->timeAccumulator = 0;
	gameloop->targetFrameTime = targetFrameTime;
	gameloop->targetTickTime = targetTickTime;

	gameloop->extraData = NULL;

	gameloop->frameStartTime = manWin.getMilliseconds();
	gameloop->frameCount = 0;
	gameloop->fps = 0;

	gameloop->tickStartTime = manWin.getMilliseconds();
	gameloop->tickCount = 0;
	gameloop->tps = 0;

	doOnNew(gameloop);

	return gameloop;
}

static void enterGameLoop(GameLoop* gameloop) {
	doOnInitWindow(gameloop);

	if (!manWin.isOpen(gameloop->primaryWindow))
		manWin.openWindow(gameloop->primaryWindow);

	doOnInitOpenGL(gameloop);
	doOnInitMisc(gameloop);


	manWin.update(gameloop->primaryWindow);
	while(manWin.isOpen(gameloop->primaryWindow)) {
		doOnRender(gameloop);

		gameloop->timeAccumulator += gameloop->targetFrameTime;

		while(gameloop->timeAccumulator >= gameloop->targetTickTime) {
			gameloop->timeAccumulator -= gameloop->targetTickTime;

			doOnUpdate(gameloop);

			gameloop->tickCount++;
			double currentTime = manWin.getMilliseconds();
			if (currentTime-gameloop->tickStartTime >= 1000) {
				gameloop->tps = (double)gameloop->tickCount*((currentTime-gameloop->tickStartTime)/1000.0);
				gameloop->tickStartTime = currentTime;
				gameloop->tickCount = 0;
			}
		}
		manWin.update(gameloop->primaryWindow);

		manWin.swapBuffers(gameloop->primaryWindow);

		gameloop->frameCount++;
		double currentTime = manWin.getMilliseconds();
		if (currentTime-gameloop->frameStartTime >= 1000) {
			gameloop->fps = (double)gameloop->frameCount*((currentTime-gameloop->frameStartTime)/1000.0);
			gameloop->frameStartTime = currentTime;
			gameloop->frameCount = 0;
		}
	}

	doOnClose(gameloop);
}

static void delete(GameLoop* gameloop) {
	doOnDestroy(gameloop);

	manWin.delete(gameloop->primaryWindow);
	free(gameloop);
}

const GameLoopManager manGameLoop = {new, enterGameLoop, delete};
