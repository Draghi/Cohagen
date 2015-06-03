#include "Ship.h"

#include "util/ObjLoader.h"
#include "util/TextureUtil.h"
#include "math/Quat.h"

typedef struct ShipData_s {
	Quat rotation;
} ShipData;

static void onUpdate(GameObject* self, float tickDelta) {
	ShipData* data = (ShipData*) self->parent;

	self->rotation.x += manMouse.getDY(self->window)/100;
	if (self->rotation.x > 0.174532925) {
		self->rotation.x = 0.174532925;
	} else if (self->rotation.x < -0.174532925) {
		self->rotation.x = -0.174532925;
	}

	self->rotation.y += manMouse.getDX(self->window)/100;
}

static void onRender(GameObject* self, float frameDelta, Shader* shader, MatrixManager* matMan) {

}

static void onCollision(GameObject* self, GameObject* other) {

}

GameObject* newShip(Window* window, Shader* shader) {
	GameObject* ship = manGameObj.new("playerShip", NULL, true, true, onUpdate, onCollision, onRender, NULL, window);
	ship->parent = malloc(sizeof(ShipData));

	manGameObj.setPhysicsCollider(ship, objLoader.loadCollisionMesh("./data/models/Boxy.col.obj", NULL, NULL, NULL, NULL));

	int vPos = manShader.getAttribLocation(shader, "vPos");
	int vNorm = manShader.getAttribLocation(shader, "vNorm");
	int vTex = manShader.getAttribLocation(shader, "vTex");

	VAO* vao = objLoader.genVAOFromFile("./data/models/Boxy.obj", vPos, vNorm, vTex);
	Texture* tex = textureUtil.createTextureFromFile("./data/texture/hourglass_front.bmp", GL_LINEAR, GL_LINEAR);

	RenderObject* ro = manRenderObj.new(NULL, NULL, NULL);
	manRenderObj.setModel(ro, vao);
	manRenderObj.addTexture(ro, tex);
	manGameObj.setModel(ship, ro);

	return ship;
}
