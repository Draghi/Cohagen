#include "Tests.h"

#include "../input.h"
#include "../graphics.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

/**The rotation angle of the cubes*/
static float rot = 0;
/**The camera's x rotation*/
static float camRotX = 45;
/**The camera's y rotation*/
static float camRotY = 0;
/**The camera's distance from the origin*/
static float dist = 15.0;

/** The teapot's texture */
static Texture *texture;
/** The vbo containing the vertex's for the cubes */
static VBO* vbo;
/** The vbo containing the colours for the cubes */
static VBO* vboc;

/** Sets the camera for the scene */
static void setCamera() {
	//Rotate the camera on the spot so it faces down the x axis
 	glRotatef(-90, 0.0, 1.0, 0.0);
 	//Move the camera a certain distance away
	glTranslatef(-dist, 0.0, 0.0);
	//rotate the camera about the origin
 	glRotatef(camRotX, 0.0, 0.0, -1.0);
 	glRotatef(camRotY, 0.0, 1.0, 0.0);
 	//Moves up two more so things look a little nicer.
 	glTranslatef(0.0, -2.0, 0.0);
}

/** Binds the vbos for rendering */
static void prepareCubeVBOs() {
	/*//Setup the vertex VBO
	glEnableClientState(GL_VERTEX_ARRAY);
	vboManager.bind(vbo);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	//Setup the colour vbo
	glEnableClientState(GL_COLOR_ARRAY);
	vboManager.bind(vboc);
	glColorPointer(3, GL_FLOAT, 0, 0);*/
}

/**
 * Renders a spinning textured teapot of the given size.
 * @param size The size of the teapot to render.
 */
static void renderTexturedTeapot(int size) {
	//Enable texturing and disable backface culling
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glPushMatrix();
		//set the texture to use the first texture unit
		glTex.bind(texture, 0);

	 	//Move it above the cubes
		glTranslatef(0, 5.0, 0);
		//Roate the teapot at a 10th the speed of the cubes
	 	glRotatef(-rot/10, 0.0, 1.0, 0.0);

		//draw the teapot
		glutSolidTeapot(size);

		//reset the texture unit
		glTex.unbind(texture);
	glPopMatrix();

	//Re-enable backface culling and enable texturing
	glEnable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

/**
 * Renders an x by x grid of spinning cubes, where x is the given size.
 * @param size The size fo the grid to render.
 */
static void renderCubeGrid(int size) {
	 int i = 0;
	 int j = 0;
	 for(i=0; i<size; i++) {
		 for(j=0; j<size; j++) {
			 glPushMatrix();
			 	 //Set the position of the cube
				 glTranslatef(i*4-(size/2*4-0.5), -2.0, -(j*4-(size/2*4-0.5)));
				 //Scale it to be size 2
				 glScalef(2.0f, 2.0f, 2.0f);
				 //Rotate it around
				 glRotatef(rot, 0.0, 1.0, 0.0);
				 glRotatef(-rot, 1.0, 0.0, 0.0);
				 glRotatef(rot/10, 0.0, 0.0, 1.0);

				 glVBO.draw(vbo, NULL, vboc, NULL);
			 glPopMatrix();
		 }
	 }
}
/**
 * Renders a spinning wire teapot of the given size.
 * @param size The size of the teapot to render.
 */
static void renderWireTeapot(int size) {
	glPushMatrix();
		//Move the teapot above the cubes
	 	glTranslatef(0, 5.0, 0);
	 	//Rotate it at about 1/10th the speed of the cubes
		glRotatef(-rot/10, 0.0, 1.0, 0.0);
		//Render the wireframe
		glutWireTeapot(size);
	glPopMatrix();
}

/** The render loop */
static void render() {
	//Clear the screen and reset the draw colour, just incase.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glColor4f(1.0, 1.0, 1.0, 1.0);

	glLoadIdentity();
	glPushMatrix();
		//Set the rotations for the scene camera
		setCamera();
		//Prepare the cube VBOs for rendering
		prepareCubeVBOs();

		//Render an 80 by 80 cube grid
		renderCubeGrid(80);
		//Render a textured teapot and wire teapot at the same place.
		renderTexturedTeapot(5);
		renderWireTeapot(5);
    glPopMatrix();
}

/** The main update loop */
static void update() {
	//Lets us reduce the size of the movements when we're further away giving us more control.
	float distMod = (10/(10+dist/4));

	//"Click-and-drag" style camera movement
	if (mouse.isButtonDown(GLUT_LEFT_BUTTON)) {
		camRotX += mouse.getYMove()*distMod;
		camRotY += mouse.getXMove()*distMod;
	}

	//Adjust the view dist via the scollwheel and ensure we are >=0
	dist = fmaxf(dist - mouse.getScrollCount()/(2*distMod), 0);

	//If w and s are not pressed, rotate forward slowly
	//If w is pressed and s is not, rotate forwards at 3x the speed.
	//If s is pressed and w is not, rotate backwards at 3x the speed
	//If both w and s are pressed, do not rotate.
	if (keyboard.isKeyDown('w')) {
		if (!keyboard.isKeyDown('s')) {
			rot += 3;
		}
	} else if (keyboard.isKeyDown('s')) {
		rot -= 3;
	} else {
		rot++;
	}

	//Make sure rot is in the range of 0<=rot<3600
	rot = fmodf(rot, 3600.0f);

	//Set the title of the window to the current FPS
	char title[64];
	sprintf(title, "FPS: %d", display.getFPS());
	display.setWindowTitle(title);
}

/** Called when the window is resized to re-create the matrix/viewport */
static void onResize(int32_t w, int32_t h) {
	//set the viewport to occupy the window screen
    glViewport(0, 0, w, h);

	//Rebuild our fixed function pipline matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,w/(float)h,0.1,500);
    glMatrixMode(GL_MODELVIEW);
}

//Need to be defined here, or we could malloc them in the function bellow.
static GlutMainCallbacks mcb = {update, render};
static GlutWindowCallbacks wcb = {onResize};

/**Prepares display/glut callbacks */
static void setupCallbacks() {
	//Setup the callbacks for the unit test
	display.setMainCallbacks(&mcb);
	display.setWindowCallbacks(&wcb);
}

/** Prepares textures fot the example */
static void setupTextures() {
	//Create a texture full of random rgba values for our solid teapot.
	texture = glTex.new();
	glTex.genData(texture, TEX_GEN_NOISE, 128, 96, GL_RGBA8, GL_RGBA, GL_NEAREST, GL_NEAREST);
}

/** Prepares VBOs for the example */
static void setupVBOs() {
    vbo = glVBO.new();

    //The data for a cube with side lengths of 1 unit and the origin at it's center
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

    //Upload the data to the gpu
    glVBO.setData(vbo, dat, sizeof(dat), GL_STATIC_DRAW);
    glVBO.setRenderInfo(vbo, sizeof(dat)/(sizeof(float)*3), 3, 0, NULL);

    vboc = glVBO.new();
    //Just add 0.5 to all values in dat and we'l get a nice smooth gradient on our cube.
    float dat2[108];
    for(int i = 0; i<108; i++) {
    	dat2[i] = dat[i]+0.5f;
    }

    //Upload the data to the GPU
    glVBO.setData(vboc, dat2, sizeof(dat2), GL_STATIC_DRAW);
    glVBO.setRenderInfo(vboc, sizeof(dat)/(sizeof(float)*3), 3, 0, NULL);
}

/** Called to run the teapot/cube example, must be called after display init */
void runTeapotCubes() {
	setupCallbacks();
	setupTextures();
	setupVBOs();

	//Fire up glut!
    glutMainLoop();
}
