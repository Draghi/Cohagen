#include "Asteroid.h"

#include "util/ObjLoader.h"
#include "util/TextureUtil.h"

static VAO* vao;
static Texture* tex;
static RenderObject* ro;
static PhysicsCollider* col;

void prepareAsteroids(Shader* shader) {
	int vPos = manShader.getAttribLocation(shader, "vPos");
	int vNorm = manShader.getAttribLocation(shader, "vNorm");
	int vTex = manShader.getAttribLocation(shader, "vTex");
	vao = objLoader.genVAOFromFile("./data/models/meteor.obj", vPos, vNorm, vTex);
	tex = textureUtil.createTextureFromFile("./data/texture/asteroid.bmp", GL_LINEAR, GL_LINEAR);
	ro = manRenderObj.new(NULL, NULL, NULL);
	col = objLoader.loadCollisionMesh("./data/models/meteor.col.obj", NULL, NULL, NULL, NULL);
}

GameObject* newAsteroid(Window* window, Vec3 pos, Vec3 rot, Vec3 scl) {
	GameObject* asteroid = manGameObj.new("Asteroid", NULL, true, true, NULL, NULL, NULL, NULL, window);
	manGameObj.setPositionVec(asteroid, &pos);
	manGameObj.setRotationVec(asteroid, &rot);
	manGameObj.setScaleVec(asteroid, &scl);
	manRenderObj.setModel(ro, vao);
	manRenderObj.addTexture(ro, tex);
	manGameObj.setModel(asteroid, ro);

	manGameObj.setPhysicsCollider(asteroid, col);
	asteroid->particle->damping = 0.9;

	return asteroid;
}
