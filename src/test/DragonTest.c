	#include <stdio.h>
#include <math.h>

#include "DragonTest.h"

#include <math.h>

#include "lib/ogl.h"
#include "glut/Display.h"
#include "math/Mat4.h"
#include "math/Vec3.h"
#include "util/ObjLoader.h"
#include "util/OGLUtil.h"
#include "gl/Shader.h"
#include "physics/Particle.h"
#include "physics/ParticleForceRegistry.h"
#include "physics/GravityForceGenerator.h"
#include "physics/AnchoredSpringForceGenerator.h"

#include <GL/glut.h>

static void setupDragonDisplay();
static void setupDragonOpenGL();
static void loadDragonResources();
static void dragonDisplay();
static void dragonUpdate();
static Mat4 createProjectionMatrix(float verticalFovRad, float nearZClip, float farZClip, float aspectRatio) ;

static GLuint vao;
static int numIndicesToDraw;
static Shader *dragonShader;
static Mat4 worldMatrix;
static Mat4 projMatrix;
static Particle *dragonParticle;
static ParticleForceRegistry *particleForceRegistry;
static GravityForceGenerator *gravityFG;
static AnchoredSpringForceGenerator *springFG;

void dragonTest() {
	setupDragonDisplay();
	setupDragonOpenGL();
	loadDragonResources();

	printf("My Dragon test\n");
}

static void dragonDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manShader.bind(dragonShader);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	manShader.unbind();
}

static void dragonUpdate(uint32_t delta) {
	char buf[64];
	snprintf(buf, 64, "FPS: %d | TPS: %d | Ticks/Frames: %f", display.getFPS(), display.getTPS(), display.getTPS()/(float)display.getFPS());
	display.setWindowTitle(buf);

	// Needed to prevent particle trying to integrate over an infinite time step when TPS = 0 (happens on startup)
	int tps = display.getTPS() == 0 ? 400 : display.getTPS();

	// Update forces acting on particle
	manForceRegistry.updateForces(particleForceRegistry, 1 / (float) tps);

	// Update particle
	manParticle.integrate(dragonParticle, 1 / (float) tps);

	// printf("%f %f %f\n", dragonParticle->position.x, dragonParticle->position.y, dragonParticle->position.z);

	// Update world matrix (position of dragon)
	worldMatrix.data[3].x = dragonParticle->position.x;
	worldMatrix.data[3].y = dragonParticle->position.y;
	worldMatrix.data[3].z = dragonParticle->position.z;
	manShader.bindUniformMat4(dragonShader, "worldMatrix", &worldMatrix);

	// Update projection matrix
	projMatrix = createProjectionMatrix(1.152f, 0.1f, 100.0f, display.getWindowWidth()/(float)display.getWindowHeight());
	manShader.bindUniformMat4(dragonShader, "projMatrix", &projMatrix);

	// Update viewport in case of display change
	glViewport(0, 0, display.getWindowWidth(), display.getWindowHeight());

	// Vec3 force = manVec3.create(NULL, 0.0f, -1.0f, 0.0f);
	// manParticle.addForce(dragonParticle, &force);
}

static void setupDragonDisplay() {
	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center

	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE); //Double buffered, RGB + depth buffer, Multisampled window.
	display.setOGLVersion(3, 3); //Set the context to OGL 3.3

	display.setVirtualFPS(60); //Sets the virtual FPS
	display.setVirtualTPS(60); //Sets the virtual TPS

	display.setUpdateCallback(dragonUpdate);
	display.setRenderCallback(dragonDisplay);

	display.createWindow(); //Create the window
}

static void setupDragonOpenGL() {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void loadDragonResources() {
	vao = objLoader.genVAOFromFile("./build/data/models/dragon_smooth.obj", &numIndicesToDraw);
	printf("VAO ID: %d Vertex Count: %d\n", vao, numIndicesToDraw);

	dragonShader = manShader.newFromGroup("./build/data/shaders/", "dragon");

	// Bind matrices
	worldMatrix = manMat4.createLeading(NULL, 1.0f);

	Mat4 viewMatrix = manMat4.createLeading(NULL, 1.0f);
	viewMatrix.data[3].z = -10.0f;

	projMatrix = createProjectionMatrix(1.152f, 0.1f, 100.0f, display.getWindowWidth()/(float)display.getWindowHeight());
	// projMatrix = manMat4.createLeading(NULL, 1.0f);

	manShader.bindUniformMat4(dragonShader, "worldMatrix", &worldMatrix);
	manShader.bindUniformMat4(dragonShader, "projMatrix", &projMatrix);
	manShader.bindUniformMat4(dragonShader, "viewMatrix", &viewMatrix);

	// Load physics sub-system
	dragonParticle = manParticle.new();
	dragonParticle->position = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);

	particleForceRegistry = manForceRegistry.new();

	Vec3 gravityAccel = manVec3.create(NULL, 0.0f, -0.1f, 0.0f);
	gravityFG = manGravityForceGenerator.new(&gravityAccel);

	Vec3 anchor = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);
	springFG = manAnchoredSpringForceGenerator.new(&anchor, 0.1f, 2.0f);

	manForceRegistry.add(particleForceRegistry, dragonParticle, &(gravityFG->forceGenerator));
	manForceRegistry.add(particleForceRegistry, dragonParticle, &(springFG->forceGenerator));
}

static Mat4 createProjectionMatrix(float verticalFovRad, float nearZClip, float farZClip, float aspectRatio) 
{
	float range = tan(verticalFovRad * 0.5f) * nearZClip;
	
	float Sx = (2.0f * nearZClip) / (range * aspectRatio + range * aspectRatio);
	float Sy = nearZClip / range;
	float Sz = -(farZClip + nearZClip) / (farZClip - nearZClip);
	float Pz = -(2.0f * farZClip * nearZClip) / (farZClip - nearZClip);

	return (manMat4.create(
		NULL,
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, Pz,
		0.0f, 0.0f, -1.0f, 0.0f));
}
