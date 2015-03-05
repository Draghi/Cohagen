#include "glut/Display.h"
#include"glut/GlutCallbacks.h"
#include <GL/glut.h>

GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{ 1.0,-1.0,-1.0},{ 1.0, 1.0,-1.0},{-1.0, 1.0,-1.0},
						 {-1.0,-1.0, 1.0},{ 1.0,-1.0, 1.0},{ 1.0, 1.0, 1.0},{-1.0, 1.0, 1.0}};

GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},{1.0,1.0,0.0},{0.0,1.0,0.0},
					   {0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}};

/**@todo Remove testing code.*/
float rot = 0;

/**@todo Remove testing code.*/
static void polygon(int a, int b, int c , int d) {
	glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glVertex3fv(vertices[a]);
		glColor3fv(colors[b]);
		glVertex3fv(vertices[b]);
		glColor3fv(colors[c]);
		glVertex3fv(vertices[c]);
		glColor3fv(colors[d]);
		glVertex3fv(vertices[d]);
	glEnd();
}

/**@todo Remove testing code.*/
static void colorcube() {
	polygon(0,3,2,1);
	polygon(2,3,7,6);
	polygon(0,4,7,3);
	polygon(1,2,6,5);
	polygon(4,5,6,7);
	polygon(0,1,5,4);
}

/**@todo Remove testing code.*/
static void render() {
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 glLoadIdentity();
	 	 int i = 0;
	 	 int j = 0;
	 	 for(i=0; i<3; i++) {
		 	 for(j=0; j<3; j++) {
				 glPushMatrix();
				 	 //"Camera"
				 	 glTranslatef(0.0, -4.0, 0.0);
				 	 glRotatef(45, 1.0, 0.0, 0.0);

				 	 //"Model"
					 glTranslatef(i*3-3, -2.0, -(5.75+j*3));
					 glRotatef(rot, 0.0, 1.0, 0.0);
					 colorcube();
				 glPopMatrix();
		 	 }
	 	 }
	 glFlush();

	 glutSwapBuffers();
}

/**@todo Remove testing code.*/
static void update() {
	rot++;

	if (rot>=360)
		rot = 0;
}

/**@todo Remove testing code.*/
static void onResize(int32_t w, int32_t h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,w/(float)h,0.1,100);

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

    glutMainLoop();

	return 0;
}
