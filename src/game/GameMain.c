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

static void initMatMan(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	data->matMan = manMatMan.new();
	manMatMan.setMode(data->matMan, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(data->matMan, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.001, 10000);
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
		manShader.bindUniformFloat(data->globalShader, "near", 0.001);
		manShader.bindUniformFloat(data->globalShader, "FCoef", 2.0/log(10000*0.001 + 1));
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
	data->quitScreen->position->z = -smallestDimention/(tan(0.5*1.152f));
}

static void initSkybox(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	data->skybox = manSkybox.new("./data/texture/purplenebula_front.bmp", "./data/texture/purplenebula_back.bmp",
							         "./data/texture/purplenebula_top.bmp",   "./data/texture/purplenebula_top.bmp",
								     "./data/texture/purplenebula_left.bmp",  "./data/texture/purplenebula_right.bmp");
	data->skyboxShader = manShader.newFromGroup("./data/shaders/", "skybox");
}

static void onInitMisc(GameLoop* self) {
	GameData* data = (GameData*)self->extraData;

	data->mainCamera = manCamera.new(NULL, NULL, NULL);

	initMatMan(self);
	initGlobalShader(self);
	initEndScreen(self);
	initSkybox(self);

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
		data->mainCamera->rotation.x -= manMouse.getDY(self->primaryWindow)/100;
		data->mainCamera->rotation.y += manMouse.getDX(self->primaryWindow)/100;

		if (manKeyboard.isDown(self->primaryWindow, KEY_ESCAPE)) {
			data->gameState = QUIT_STATE;
			data->escStilDown = true;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

	} else if (data->gameState == QUIT_STATE) {
		if (manKeyboard.isDown(self->primaryWindow, KEY_ESCAPE)) {
			if (!data->escStilDown)
				exit(0);
		} else {
			data->escStilDown = false;
		}
	}
}

static void bindMatricies(Shader* sha, MatrixManager* mats) {
	manShader.bindUniformMat4(sha, "projectionMatrix", manMatMan.peekStack(mats, MATRIX_MODE_PROJECTION));
	manShader.bindUniformMat4(sha, "viewMatrix",       manMatMan.peekStack(mats, MATRIX_MODE_VIEW));
	manShader.bindUniformMat4(sha, "modelMatrix",      manMatMan.peekStack(mats, MATRIX_MODE_MODEL));
}

static const Vec3 xAxis = {1, 0, 0};
static const Vec3 yAxis = {0, 1, 0};
static const Vec3 zAxis = {0, 0, 1};

static void setupCamera(MatrixManager* mats, Vec3* camPos, Vec3* camRot) {
	manMatMan.rotate(mats, camRot->x, xAxis);
	manMatMan.rotate(mats, camRot->y, yAxis);
	manMatMan.rotate(mats, camRot->z, zAxis);
	manMatMan.translate(mats, *camPos);
}

static void renderSkybox(MatrixManager* manMat, Shader* skyboxShader, Skybox* skybox) {
	manShader.bind(skyboxShader);
		bindMatricies(skyboxShader, manMat);
		manSkybox.draw(skybox, skyboxShader, "cubeTexture");
	manShader.unbind();
}

static void onRender(GameLoop* self, float frameDelta) {
	GameData* data = self->extraData;
	glViewport(0, 0, manWin.getFramebufferWidth(self->primaryWindow), manWin.getFramebufferHeight(self->primaryWindow));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (data->gameState == GAME_STATE) {
		/** @todo replace with camera bind **/
		manMatMan.setMode(data->matMan, MATRIX_MODE_VIEW);
		manMatMan.push(data->matMan);
			setupCamera(data->matMan, &data->mainCamera->position, &data->mainCamera->rotation);
			renderSkybox(data->matMan, data->skyboxShader, data->skybox);
		manMatMan.setMode(data->matMan, MATRIX_MODE_VIEW);
		manMatMan.pop(data->matMan);
	} else if (data->gameState == QUIT_STATE) {
		manRenderer.renderModel(data->quitScreen, data->globalShader, data->matMan);
	}
}
