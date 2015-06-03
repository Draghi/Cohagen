#include "GameMain.h"

#include "engine/GameObjectRegistry.h"
#include "render/Camera.h"
#include "render/Renderer.h"
#include "render/Skybox.h"
#include "util/OGLUtil.h"
#include "util/ObjLoader.h"

typedef enum stateType_s {GAME_STATE, QUIT_STATE, LIGHTING_STATE} stateType;

typedef struct GameData_s {
	//Skybox Rendering
	Skybox *skybox;
	Shader *skyboxShader;

	//World Rendering
	Shader *globalShader;
	Camera *mainCamera;

	//Quit Screen
	RenderObject* quitScreen;
	Shader* quitScreenShader;

	//World
	GameObjectRegist* gameObjRegist;

	//Game
	stateType gameState;
	MatrixManager* matMan;
} GameData;

static void onCreate(GameLoop* self);
static void onInitWindow(GameLoop* self);
static void onInitOpenGL(GameLoop* self);
static void onInitMisc(GameLoop* self);
static void onClose(GameLoop* self);
static void onDestroy(GameLoop* self);
static void onUpdate(GameLoop* self, float tickDelta);
static void onRender(GameLoop* self, float frameDelta);

void runGame() {
	GameLoop* gameloop = manGameLoop.new(onCreate, onInitWindow, onInitOpenGL, onInitMisc, onUpdate, onRender, onClose, onDestroy, 1/120.0, 1/60.0);
	manGameLoop.enterGameLoop(gameloop);
	manGameLoop.delete(gameloop);
}

static void onCreate(GameLoop* self) {
	self->extraData = malloc(sizeof(GameData));
}

static void onInitWindow(GameLoop* self) {
	self->primaryWindow->isFullscreen = true;
	self->primaryWindow->shouldCaptureMouse = true;
}

static void onInitOpenGL(GameLoop* self) {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE);
	glEnable(GL_CLIP_DISTANCE0);

	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void onInitMisc(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	data->matMan = manMatMan.new();
	manMatMan.setMode(data->matMan, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(data->matMan, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 100);
	manMatMan.setMode(data->matMan, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(data->matMan);
	manMatMan.setMode(data->matMan, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(data->matMan);

	data->quitScreenShader = manShader.newFromGroup("./data/shaders/", "passThruTex");

	int posLoc  = manShader.getAttribLocation(data->quitScreenShader, "vPos");
	int normLoc = manShader.getAttribLocation(data->quitScreenShader, "vNorm");
	int texLoc  = manShader.getAttribLocation(data->quitScreenShader, "vTex");

	VAO* vao = objLoader.genVAOFromFile("./data/models/cube.obj", posLoc, normLoc, texLoc);
	Texture* tex = textureUtil.createTextureFromFile("./data/texture/quitScreen.bmp", GL_LINEAR, GL_LINEAR);

	data->quitScreen = manRenderObj.new(NULL, NULL, NULL);
	manRenderObj.setModel(data->quitScreen, vao);
	manRenderObj.addTexture(data->quitScreen, tex);

	data->gameState = GAME_STATE;
}

static void onClose(GameLoop* self) {

}

static void onDestroy(GameLoop* self) {
	free(self->extraData);
}

static void onUpdate(GameLoop* self, float tickDelta) {
	GameData* data = self->extraData;

	if (data->gameState == GAME_STATE) {
		if (manKeyboard.isDown(self->primaryWindow, KEY_ESCAPE)) {
			data->gameState = QUIT_STATE;
		}
	} else if (data->gameState == QUIT_STATE) {
	}
}

static void onRender(GameLoop* self, float frameDelta) {
	GameData* data = self->extraData;
	glViewport(0, 0, manWin.getFramebufferWidth(self->primaryWindow), manWin.getFramebufferHeight(self->primaryWindow));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (data->gameState == GAME_STATE) {

	} else if (data->gameState == QUIT_STATE) {
		manRenderer.renderModel(data->quitScreen, data->quitScreenShader, data->matMan);
	}
}
