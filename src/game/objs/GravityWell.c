#include "GravityWell.h"

static VAO* vao;
static Texture* tex;
static RenderObject* ro;
static PhysicsCollider* col;

#include "util/ObjLoader.h"
#include "util/TextureUtil.h"
#include "physics/AnchoredGravityForceGenerator.h"

void prepareGrav(Shader* shader) {
	int vPos = manShader.getAttribLocation(shader, "vPos");
	int vNorm = manShader.getAttribLocation(shader, "vNorm");
	int vTex = manShader.getAttribLocation(shader, "vTex");
	vao = objLoader.genVAOFromFile("./data/models/grav.obj", vPos, vNorm, vTex);
	tex = textureUtil.createTextureFromFile("./data/texture/quitScreen.bmp", GL_LINEAR, GL_LINEAR);
	ro = manRenderObj.new(NULL, NULL, NULL);
	col = objLoader.loadCollisionMesh("./data/models/grav.col.obj", NULL, NULL, NULL, NULL);
}

GameObject* newGrav(GameObjectRegist* regist, Window* window, Vec3 pos, Vec3 rot, Vec3 scl) {
	GameObject* grav = manGameObj.new("Grav", NULL, true, true, NULL, NULL, NULL, NULL, window);

	manGameObj.setPositionVec(grav, &pos);
	manGameObj.setRotationVec(grav, &rot);
	manGameObj.setScaleVec(grav, &scl);
	manRenderObj.setModel(ro, vao);
	manRenderObj.addTexture(ro, tex);
	manGameObj.setModel(grav, ro);

	manGameObj.setPhysicsCollider(grav, col);
	grav->particle->damping = 0;
	grav->particle->inverseMass = 1e-15;

	AnchoredGravityForceGenerator* fg = manAnchoredGravityForceGenerator.new(grav->particle);

	for(int i = 0; i<regist->gameObjects->size; i++) {
		GameObject* obj = manGameObjRegist.getGameObject(regist, i);
		manGameObj.addForceGenerator(obj, &fg->forceGenerator);
	}

	grav->physCollider->immovable = true;

	return grav;
}
