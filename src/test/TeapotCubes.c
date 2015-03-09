#include "Tests.h"

#include "../input.h"
#include "../graphics.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

static float rot = 0;
static float camRotX = 45;
static float camRotY = 0;
static float dist = 15.0;

static Texture *texture;
static VBO* vbo;
static VBO* vboc;

static void setCamera() {
 	 glRotatef(-90, 0.0, 1.0, 0.0);
	 glTranslatef(-dist, 0.0, 0.0);
 	 glRotatef(camRotX, 0.0, 0.0, -1.0);
 	 glRotatef(camRotY, 0.0, 1.0, 0.0);
 	 glTranslatef(0.0, -2.0, 0.0);
}

static void prepareCubeVBOs() {
	 glEnableClientState(GL_VERTEX_ARRAY);
	 vbo->bind(vbo);
	 glVertexPointer(3, GL_FLOAT, 0, 0);

	 glEnableClientState(GL_COLOR_ARRAY);
	 vboc->bind(vboc);
	 glColorPointer(3, GL_FLOAT, 0, 0);
}

static void renderTexturedTeapot(int size) {
	 glEnable(GL_TEXTURE_2D);
	 glDisable(GL_CULL_FACE);

	 glPushMatrix();
	 	 texture->bind(texture, 0);

	 	 glRotatef(-rot/10, 0.0, 1.0, 0.0);
		 glTranslatef(0, 5.0, 0);
		 glutSolidTeapot(size);

		 texture->unbind(texture);
	 glPopMatrix();

	 glEnable(GL_CULL_FACE);
	 glDisable(GL_TEXTURE_2D);
}

static void renderCubeGrid(int size) {
	 int i = 0;
	 int j = 0;
	 for(i=0; i<size; i++) {
		 for(j=0; j<size; j++) {
			 glPushMatrix();
				 //"Model"
				 glTranslatef(i*4-(size/2*4-0.5), -2.0, -(j*4-(size/2*4-0.5)));
				 glScalef(2.0f, 2.0f, 2.0f);
				 glRotatef(rot, 0.0, 1.0, 0.0);
				 glRotatef(-rot, 1.0, 0.0, 0.0);
				 glRotatef(rot/10, 0.0, 0.0, 1.0);

				 glDrawArrays(GL_TRIANGLES, 0, 36);
			 glPopMatrix();
		 }
	 }
}

static void renderWireTeapot(int size) {
	glPushMatrix();
		 glRotatef(-rot/10, 0.0, 1.0, 0.0);
		 glTranslatef(0, 5.0, 0);
		 glutWireTeapot(size);
	glPopMatrix();
}

static void render() {
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	 glColor4f(1.0, 1.0, 1.0, 1.0);

	 glLoadIdentity();
	 glPushMatrix();
	 	 setCamera();
	 	 prepareCubeVBOs();

	 	 renderCubeGrid(80);
	 	 renderTexturedTeapot(5);
	 	 renderWireTeapot(5);

	  glPopMatrix();
}

static void update() {

	float distMod = (10/(10+dist/4));

	if (mouse.isButtonDown(GLUT_LEFT_BUTTON)) {
		camRotX += mouse.getYMove()*distMod;
		camRotY += mouse.getXMove()*distMod;
	}

	dist = fmaxf(dist - mouse.getScrollCount()/(2*distMod), 0);

	if (keyboard.isKeyDown('w')) {
		if (!keyboard.isKeyDown('s')) {
			rot += 3;
		}
	} else if (keyboard.isKeyDown('s')) {
		rot -= 3;
	} else {
		rot++;
	}

	rot = fmodf(rot, 3600.0f);

	char title[64];
	sprintf(title, "FPS: %d", display.getFPS());
	display.setWindowTitle(title);
}

static void onResize(int32_t w, int32_t h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,w/(float)h,0.1,500);

    glMatrixMode(GL_MODELVIEW);
}

static GlutMainCallbacks mcb = {update, render};
static GlutWindowCallbacks wcb = {onResize};

static void setupCallbacks() {
	display.setMainCallbacks(&mcb);
	display.setWindowCallbacks(&wcb);
}

static void setupTextures() {
    texture = textureManager.createNoiseRGBATexture(128, 96, GL_NEAREST, GL_NEAREST);
}

static void setupVBOs() {
    vbo = vboManager.createVBO();

    float dat[] = { 0.5f, 0.5f,-0.5f,   0.5f,-0.5f,-0.5f,  -0.5f,-0.5f,-0.5f,
				   -0.5f,-0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,   0.5f, 0.5f,-0.5f,

				   -0.5f,-0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,  -0.5f,-0.5f, 0.5f,

				   -0.5f,-0.5f,-0.5f,   0.5f,-0.5f,-0.5f,   0.5f,-0.5f, 0.5f,
					0.5f,-0.5f, 0.5f,  -0.5f,-0.5f, 0.5f,  -0.5f,-0.5f,-0.5f,

			 	 	0.5f, 0.5f, 0.5f,   0.5f, 0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,
				   -0.5f, 0.5f,-0.5f,  -0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,

				   -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,-0.5f,  -0.5f,-0.5f,-0.5f,
				   -0.5f,-0.5f,-0.5f,  -0.5f,-0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,

				    0.5f,-0.5f,-0.5f,   0.5f, 0.5f,-0.5f,   0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f,-0.5f,-0.5f};

    vbo->setData(vbo, dat, sizeof(dat), GL_STATIC_DRAW);

    vboc = vboManager.createVBO();

    float dat2[108];
    for(int i = 0; i<108; i++) {
    	dat2[i] = dat[i]+0.5f;
    }

    vboc->setData(vboc, dat2, sizeof(dat2), GL_STATIC_DRAW);
}

void runTeapotCubes() {
	setupCallbacks();
	setupTextures();
	setupVBOs();

    glutMainLoop();
}
