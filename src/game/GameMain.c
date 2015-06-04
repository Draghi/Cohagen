#include "GameMain.h"

#include "game/objs/CameraController.h"
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

	//Camera
	scalar speed;

	//Quit Screen
	RenderObject* quitScreen;

	//World
	GameObjectRegist* gameObjRegist;

	//Game
	stateType gameState;
	MatrixManager* matMan;

	bool escStilDown;
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


/* *********** *
 *  Init Start *
 * *********** */
static void onInitWindow(GameLoop* self) {
	self->primaryWindow->isFullscreen = true;
	self->primaryWindow->shouldCaptureMouse = true;
}

static void onInitOpenGL(GameLoop* self) {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE);
	//glEnable(GL_CLIP_DISTANCE0);

	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void initMatMan(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	data->matMan = manMatMan.new();
	manMatMan.setMode(data->matMan, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(data->matMan, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.01, 3000);
	manMatMan.setMode(data->matMan, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(data->matMan);
	manMatMan.setMode(data->matMan, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(data->matMan);
}

static void initGlobalShader(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	data->globalShader = manShader.newFromGroup("./data/shaders/", "texLogZ");
	manShader.bind(data->globalShader);
		manShader.bindUniformInt(data->globalShader, "tex", 0);
		manShader.bindUniformFloat(data->globalShader, "near", 0.0000001);
		manShader.bindUniformFloat(data->globalShader, "FCoef", 2.0/log(3000*0.000001 + 1));
	manShader.unbind(data->globalShader);
}

static void initEndScreen(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	int posLoc  = manShader.getAttribLocation(data->globalShader, "vPos");
	int normLoc = manShader.getAttribLocation(data->globalShader, "vNorm");
	int texLoc  = manShader.getAttribLocation(data->globalShader, "vTex");

	VAO* vao = objLoader.genVAOFromFile("./data/models/cube.obj", posLoc, normLoc, texLoc);
	Texture* tex = textureUtil.createTextureFromFile("./data/texture/quitScreen.bmp", GL_LINEAR, GL_LINEAR);

	data->quitScreen = manRenderObj.new(NULL, NULL, NULL);
	manRenderObj.setModel(data->quitScreen, vao);
	manRenderObj.addTexture(data->quitScreen, tex);

	float smallestDimention = self->primaryWindow->height < self->primaryWindow->width ? self->primaryWindow->height : self->primaryWindow->width;
	data->quitScreen->scale->x = smallestDimention;
	data->quitScreen->scale->y = smallestDimention;
	data->quitScreen->scale->z = smallestDimention;
	data->quitScreen->position->z = smallestDimention+smallestDimention/(tan(0.5*1.152f));
}

static void initSkybox(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	data->skybox = manSkybox.newFromGroup("./data/texture/", "purplenebula");
	data->skyboxShader = manShader.newFromGroup("./data/shaders/", "skybox");
}

static void onInitMisc(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	initMatMan(self);
	initGlobalShader(self);
	initEndScreen(self);
	initSkybox(self);

	data->mainCamera = manCamera.new(NULL, NULL, NULL);
	manCamera.setProjectionInfo(data->mainCamera, 1.152f, 0.001, 10000);
	manCamera.setViewportObject(data->mainCamera, manViewport.new(0, 0, self->primaryWindow->width, self->primaryWindow->height));

	GameObject* cameraController = newCameraController(data->mainCamera, self->primaryWindow, 20.0f, 1000.0f, 0.125f, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_LSHIFT, KEY_E, KEY_Q);

	data->speed = 20.0f;

	data->gameObjRegist = manGameObjRegist.new(data->matMan);
	manGameObjRegist.add(data->gameObjRegist, cameraController);

	data->gameState = GAME_STATE;
}

/* ************ *
 *  Close Start *
 * ************ */
static void onClose(GameLoop* self) {

}

static void onDestroy(GameLoop* self) {
	free(self->extraData);
}

/* *********** *
 *  Game Start *
 * *********** */
static void onUpdate(GameLoop* self, float tickDelta) {
	GameData* data = self->extraData;

	if (data->gameState == GAME_STATE) {
		manGameObjRegist.update(data->gameObjRegist, tickDelta);

		/* ********* *
		 * Exit Game *
		 * ********* */
		if (manKeyboard.isDown(self->primaryWindow, KEY_ESCAPE)) {
			data->gameState = QUIT_STATE;
			data->escStilDown = true;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

	} else if (data->gameState == QUIT_STATE) {
		if (manMouse.isDown(self->primaryWindow, MOUSE_BUTTON_LEFT)) {
			exit(0);
		}
		// if (manKeyboard.isDown(self->primaryWindow, KEY_ESCAPE)) {
		// 	if (!data->escStilDown)
		// 		exit(0);
		// } else {
		// 	data->escStilDown = false;
		// }
	}
}

static void renderSkybox(MatrixManager* matMan, Shader* skyboxShader, Skybox* skybox) {
	manShader.bind(skyboxShader);
		manShader.bindUniformMat4(skyboxShader, "projectionMatrix", manMatMan.peekStack(matMan, MATRIX_MODE_PROJECTION));
		manShader.bindUniformMat4(skyboxShader, "viewMatrix",       manMatMan.peekStack(matMan, MATRIX_MODE_VIEW));
		manShader.bindUniformMat4(skyboxShader, "modelMatrix",      manMatMan.peekStack(matMan, MATRIX_MODE_MODEL));
		manSkybox.draw(skybox, skyboxShader, "cubeTexture");
	manShader.unbind();
}

static void onRender(GameLoop* self, float frameDelta) {
	GameData* data = self->extraData;
	glViewport(0, 0, manWin.getFramebufferWidth(self->primaryWindow), manWin.getFramebufferHeight(self->primaryWindow));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (data->gameState == GAME_STATE) {
		manCamera.bind(data->mainCamera, data->matMan);

			manMatMan.setMode(data->matMan, MATRIX_MODE_MODEL);
			renderSkybox(data->matMan, data->skyboxShader, data->skybox);
			manRenderer.renderModel(data->quitScreen, data->globalShader, data->matMan);

		manCamera.unbind(data->mainCamera, data->matMan);
	} else if (data->gameState == QUIT_STATE) {
		manMatMan.setMode(data->matMan, MATRIX_MODE_MODEL);
		manRenderer.renderModel(data->quitScreen, data->globalShader, data->matMan);
	}
}
