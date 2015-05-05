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
#include "gl/Textures.h"
#include "util/TextureUtil.h"

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
static Mat4 modelMatrix;
static Mat4 projMatrix;
static Particle *dragonParticle;
static ParticleForceRegistry *particleForceRegistry;
static GravityForceGenerator *gravityFG;
static AnchoredSpringForceGenerator *springFG;
static Texture *tex;

void runDragonTest() {
	setupDragonDisplay();
	setupDragonOpenGL();
	loadDragonResources();
}

static void dragonDisplay(uint32_t deltaFrame) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE);

	manShader.bind(dragonShader);
		manTex.bind(tex, 0);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		manTex.unbind(tex);
	manShader.unbind();

	glDisable(GL_TEXTURE);

	// printf("%d\n", deltaFraem);
}

static void dragonUpdate(uint32_t deltaTick) {
	char buf[64];
	snprintf(buf, 64, "FPS: %d | TPS: %d | Ticks/Frames: %f", display.getFPS(), display.getTPS(), display.getTPS()/(float)display.getFPS());
	display.setWindowTitle(buf);

	// // Needed to prevent particle trying to integrate over an infinite time step when TPS = 0 (happens on startup)
	// int tps = display.getTPS() == 0 ? 400 : display.getTPS();

	// printf("%d\n", deltaTick);
	// // Update forces acting on particle
	// manForceRegistry.updateForces(particleForceRegistry, deltaTick);

	// // Update particle
	// manParticle.integrate(dragonParticle, deltaTick);

	// Update model matrix (position of dragon)
	modelMatrix.data[3].x = dragonParticle->position.x;
	modelMatrix.data[3].y = dragonParticle->position.y;
	modelMatrix.data[3].z = dragonParticle->position.z;
	manShader.bindUniformMat4(dragonShader, "modelMatrix", &modelMatrix);

	// // Update projection matrix
	projMatrix = createProjectionMatrix(1.152f, 1.0f, 100.0f, display.getWindowWidth()/(float)display.getWindowHeight());
	manShader.bindUniformMat4(dragonShader, "projMatrix", &projMatrix);

	// Update viewport in case of display change
	glViewport(0, 0, display.getWindowWidth(), display.getWindowHeight());
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
	//Load Resources
	vao = objLoader.genVAOFromFile("./data/models/town.obj", &numIndicesToDraw);
	dragonShader = manShader.newFromGroup("./data/shaders/", "house");
	tex = textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR);

	projMatrix = createProjectionMatrix(1.152f, 1.0f, 100.0f, (float) display.getWindowWidth() / (float) display.getWindowHeight());
	manShader.bindUniformMat4(dragonShader, "projectionMatrix", &projMatrix);

	modelMatrix = manMat4.createLeading(NULL, 1.0f);
	manShader.bindUniformMat4(dragonShader, "modelMatrix", &modelMatrix);

	// Bind Texture Uniform
	glUseProgram(dragonShader->program);
	glUniform1i(glGetUniformLocation(dragonShader->program, "tex"), 0);

	// Load physics sub-system
	dragonParticle = manParticle.new();
	dragonParticle->position = manVec3.create(NULL, -4.0f, -3.0f, -19.0f);

	particleForceRegistry = manForceRegistry.new();

	Vec3 gravityAccel = manVec3.create(NULL, 0.0f, -0.1f, 0.0f);
	gravityFG = manGravityForceGenerator.new(&gravityAccel);

	Vec3 anchor = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);
	springFG = manAnchoredSpringForceGenerator.new(&anchor, 0.1f, 2.0f);

	manForceRegistry.add(particleForceRegistry, dragonParticle, &(gravityFG->forceGenerator));
	// manForceRegistry.add(particleForceRegistry, dragonParticle, &(springFG->forceGenerator));
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
