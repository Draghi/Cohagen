#include "GravityWell.h"

#include <string.h>

static RenderObject* ro1;
static RenderObject* ro2;
static PhysicsCollider* col;

#include "util/ObjLoader.h"
#include "util/TextureUtil.h"
#include "physics/AnchoredGravityForceGenerator.h"

void prepareGrav(Shader* shader) {
	int vPos = manShader.getAttribLocation(shader, "vPos");
	int vNorm = manShader.getAttribLocation(shader, "vNorm");
	int vTex = manShader.getAttribLocation(shader, "vTex");
	VAO* vao = objLoader.genVAOFromFile("./data/models/grav.obj", vPos, vNorm, vTex);
	Texture* tex1 = textureUtil.createTextureFromFile("./data/texture/grav.bmp", GL_LINEAR, GL_LINEAR);
	Texture* tex2 = textureUtil.createTextureFromFile("./data/texture/antigrav.bmp", GL_LINEAR, GL_LINEAR);
	ro1 = manRenderObj.new(NULL, NULL, NULL);
	ro2 = manRenderObj.new(NULL, NULL, NULL);
	col = objLoader.loadCollisionMesh("./data/models/grav.col.obj", NULL, NULL, NULL, NULL);

	manRenderObj.setModel(ro1, vao);
	manRenderObj.addTexture(ro1, tex1);

	manRenderObj.setModel(ro2, vao);
	manRenderObj.addTexture(ro2, tex2);
}

GameObject* newGrav(GameObjectRegist* regist, Window* window, scalar mass, Vec3 pos, Vec3 rot, Vec3 scl) {
	GameObject* grav = manGameObj.new("Grav", NULL, true, true, NULL, NULL, NULL, NULL, window);

	manGameObj.setPositionVec(grav, &pos);
	manGameObj.setRotationVec(grav, &rot);
	manGameObj.setScaleVec(grav, &scl);

	if (mass>0)
		manGameObj.setModel(grav, ro1);
	else
		manGameObj.setModel(grav, ro2);


	manGameObj.setPhysicsCollider(grav, col);
	grav->particle->damping = 0;
	manParticle.setMass(grav->particle, mass);

	AnchoredGravityForceGenerator* fg = manAnchoredGravityForceGenerator.new(grav->particle);

	for(int i = 0; i<regist->gameObjects->size; i++) {
		GameObject* obj = manGameObjRegist.getGameObject(regist, i);
		manGameObj.addForceGenerator(obj, &fg->forceGenerator);
	}

	grav->physCollider->immovable = true;

	return grav;
}
