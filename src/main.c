#include "glut/Display.h"
#include "glut/Mouse.h"
#include "glut/Keyboard.h"
#include "glut/GlutCallbacks.h"
#include "gl/Textures.h"
#include "gl/VBO.h"
#include "graphics.h"

#include <time.h>
#include <string.h>
#include <stdio.h>

/**@todo Remove testing code.*/
float rot = 0;
float camRotX = 45;
float camRotY = 0;
float dist = 15.0;
Texture *texture;
VBO* vbo;
VBO* vboc;

/**@todo Remove testing code.*/
static void render() {
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	 glColor4f(1.0, 1.0, 1.0, 1.0);
	 glLoadIdentity();
	 glPushMatrix();
 	 	 //"Camera"
 	 	 glRotatef(-90, 0.0, 1.0, 0.0);
	 	 glTranslatef(-dist, 0.0, 0.0);
 	 	 glRotatef(camRotX, 0.0, 0.0, -1.0);
 	 	 glRotatef(camRotY, 0.0, 1.0, 0.0);
 	 	 glTranslatef(0.0, -2.0, 0.0);


		 glEnableClientState(GL_VERTEX_ARRAY);
		 vbo->bind(vbo);
		 glVertexPointer(3, GL_FLOAT, 0, 0);

		 glEnableClientState(GL_COLOR_ARRAY);
		 vboc->bind(vboc);
		 glColorPointer(3, GL_FLOAT, 0, 0);

 	 	 int size = 80;
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

 	 	 glColor4f(1.0, 1.0, 1.0, 1.0);
	 	 glEnable(GL_TEXTURE_2D);
	 	 glDisable(GL_CULL_FACE);

	 	 glFrontFace(GL_CW);
		 glPushMatrix();
		 	 texture->bind(texture, 0);

		 	 glRotatef(-rot/10, 0.0, 1.0, 0.0);
			 glTranslatef(0, 5.0, 0);
			 glutSolidTeapot(5);

			 texture->unbind(texture);
		 glPopMatrix();

		 glFrontFace(GL_CCW);
		 glEnable(GL_CULL_FACE);
		 glDisable(GL_TEXTURE_2D);

	 	 glColor4f(1.0, 1.0, 1.0, 1.0);
		 glPushMatrix();
			 glRotatef(-rot/10, 0.0, 1.0, 0.0);
			 glTranslatef(0, 5.0, 0);
			 glutWireTeapot(5);
		 glPopMatrix();

	 glPopMatrix();
	 glFlush();

	 glutSwapBuffers();
}

/**@todo Remove testing code.*/
static void update() {
	rot++;

	if (mouse.isButtonDown(GLUT_LEFT_BUTTON)) {
		camRotX += mouse.getYMove();
		camRotY += mouse.getXMove();
	}

	dist -= mouse.getScrollCount();
	if (dist<0)
		dist=0;

	if (keyboard.isKeyDown('w')) {
		rot += 2;
	}

	if (keyboard.isKeyDown('s')) {
		rot -= 4;
	}

	if (rot>=3600)
		rot -= 3600;

	if (rot<0)
		rot += 3600;
	char title[64];
	strcpy(title, "FPS: ");
	sprintf(title, "FPS: %d", display.getFPS());
	display.setWindowTitle(title);
}

/**@todo Remove testing code.*/
static void onResize(int32_t w, int32_t h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,w/(float)h,0.1,500);

    glMatrixMode(GL_MODELVIEW);
}

/**@todo Remove testing code.*/
int main(int argc, char **argv) {
	glutInit(&argc, argv); //Required call to let glut setup

	//Double Buffering, RGB buffer, Depth buffer, Multisampling
	display.setDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);

	display.setWindowSize(800, 600);
	display.doCenterWindow();

	display.setOGLVersion(3, 0);

	display.setWindowTitle("Window");
	display.createWindow(&argc, argv);

	GlutMainCallbacks mcb = {update, render};
	display.setMainCallbacks(&mcb);

	GlutWindowCallbacks wcb = {onResize};
	display.setWindowCallbacks(&wcb);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    srand((unsigned)time(NULL));


    texture = textureManager.createNoiseRGBATexture(128, 96, GL_NEAREST, GL_NEAREST);

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

    glutMainLoop();

	return 0;
}
