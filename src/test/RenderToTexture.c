#include"Tests.h"
#include"../graphics.h"
#include"../input.h"
#include"../gl/FBO.h"
#include"../gl/RBO.h"

#include<math.h>
#include<stdio.h>


/**The rotation angle of the cubes*/
static float rot = 0;
/**The camera's x rotation*/
static float camRotX = 45;
/**The camera's y rotation*/
static float camRotY = 0;
/**The camera's distance from the origin*/
static float dist = 15.0;

static Texture* texture;

/** The vbo containing the vertex's for the cubes */
static VBO* vbo;
/** The vbo containing the colours for the cubes */
static VBO* vboc;
/** The vbo containing the texcoords for the cubes */
static VBO* vbot;

static RBO* rbo;
static FBO* fbo;

/** Sets the camera for the scene */
static void setCamera() {
	//Rotate the camera on the spot so it faces down the x axis
 	glRotatef(-90, 0.0, 1.0, 0.0);
 	//Move the camera a certain distance away
	glTranslatef(-dist, 0.0, 0.0);
	//rotate the camera about the origin
 	glRotatef(camRotX, 0.0, 0.0, -1.0);
 	glRotatef(camRotY, 0.0, 1.0, 0.0);
}

/**
 * Renders an x by x grid of spinning cubes, where x is the given size.
 * @param size The size fo the grid to render.
 */
static void renderCubeGrid(int size, VBO* vert, VBO* ind, VBO* colour, VBO* texcoord) {
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

					 manVBO.draw(vert, NULL, colour, texcoord);
				 glPopMatrix();
		 }
	 }
}

static void renderTeapotGrid(int size) {
	 int i = 0;
	 int j = 0;
	 for(i=0; i<size; i++) {
		 for(j=0; j<size; j++) {
				 glPushMatrix();
					 //Set the position of the cube
					 glTranslatef(i*4-(size/2*4-0.5), 0.0, -(j*4-(size/2*4-0.5)));
					 //Scale it to be size 2
					 glScalef(-1.0f, -1.0f, -1.0f);
					 //Rotate it around
					 glRotatef(rot, 0.0, 1.0, 0.0);
					 glRotatef(-rot, 1.0, 0.0, 0.0);
					 glRotatef(rot/10, 0.0, 0.0, 1.0);

					 glutSolidTeapot(1.5);
				 glPopMatrix();
		 }
	 }
}

/** The render loop */
static void render() {
	//Clear the screen and reset the draw colour, just incase.

    glViewport(0, 0, texture->width, texture->height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,texture->width/(float)texture->height,0.1,500);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	manFBO.bind(fbo);
		glClearColor(1.0,1.0,1.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
 			glTranslatef(0.0, -10.0, 0.0);
			//glRotatef(rot, 1.0, 0.0, 0.0);
 			glRotatef(rot, 0.0, 1.0, 0.0);
 			//glRotatef(rot, 0.0, 0.0, 1.0);
			renderCubeGrid(20, vbo, NULL, vboc, NULL);
		glPopMatrix();
	manFBO.unbind(fbo);

    glViewport(0, 0, display.getWindowWidth(), display.getWindowHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,display.getWindowWidth()/(float)display.getWindowHeight(),0.1,500);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		setCamera();

		glEnable(GL_TEXTURE_2D);
			manTex.bind(texture, 0);
			renderTeapotGrid(3);
			glTranslatef(0.0, 6.0, 0.0);
			glScalef(5.0, 5.0, 5.0);
			manVBO.draw(vbo, NULL, NULL, vbot);
			manTex.unbind(texture);
		glDisable(GL_TEXTURE_2D);
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

	//Rebuild our fixed function pipline matrix
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
	texture = manTex.new();
	manTex.genData(texture, TEX_GEN_BLACK, 512, 512, GL_RGBA8, GL_RGBA, GL_NEAREST, GL_NEAREST);
}

/** Prepares VBOs for the example */
static void setupVBOs() {
    vbo = manVBO.new();

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
    manVBO.setData(vbo, dat, sizeof(dat), GL_STATIC_DRAW);
    manVBO.setRenderInfo(vbo, sizeof(dat)/(sizeof(float)*3), 3, 0, NULL);

    vboc = manVBO.new();
    //Just add 0.5 to all values in dat and we'l get a nice smooth gradient on our cube.
    float dat2[108];
    for(int i = 0; i<108; i++) {
    	dat2[i] = dat[i]+0.5f;
    }

    //Upload the data to the GPU
    manVBO.setData(vboc, dat2, sizeof(dat2), GL_STATIC_DRAW);
    manVBO.setRenderInfo(vboc, sizeof(dat2)/(sizeof(float)*3), 3, 0, NULL);

    vbot = manVBO.new();

    float dat3[72] = {0.0f, 0.0f,     0.0f, 1.0f,     1.0f, 1.0f,
    				  1.0f, 1.0f,     1.0f, 0.0f,     0.0f, 0.0f,

					  0.0f, 1.0f,     1.0f, 1.0f,     1.0f, 0.0f,
					  1.0f, 0.0f,     0.0f, 0.0f,     0.0f, 1.0f,

					  0.0f, 0.0f,     0.0f, 1.0f,     1.0f, 1.0f,
					  1.0f, 1.0f,     1.0f, 0.0f,     0.0f, 0.0f,

					  1.0f, 0.0f,     0.0f, 0.0f,     0.0f, 1.0f,
					  0.0f, 1.0f,     1.0f, 1.0f,     1.0f, 0.0f,

					  1.0f, 0.0f,     0.0f, 0.0f,     0.0f, 1.0f,
					  0.0f, 1.0f,     1.0f, 1.0f,     1.0f, 0.0f,

					  1.0f, 1.0f,     1.0f, 0.0f,     0.0f, 0.0f,
					  0.0f, 0.0f,     0.0f, 1.0f,     1.0f, 1.0f};

    //Upload the data to the gpu
    manVBO.setData(vbot, dat3, sizeof(dat3), GL_STATIC_DRAW);
    manVBO.setRenderInfo(vbot, sizeof(dat3)/(sizeof(float)*2), 2, 0, NULL);
}

static void setupRBOs() {
	rbo = manRBO.new();
	manRBO.setStorage(rbo, GL_DEPTH_COMPONENT32, texture->width, texture->height);
}

static void setupFBOs() {
	fbo = manFBO.new();
	manFBO.attachRBO(fbo, rbo, GL_DEPTH_ATTACHMENT);
	manFBO.attachTexture(fbo, texture, GL_COLOR_ATTACHMENT0);
}

void runRenderToTexture() {
	setupCallbacks();
	setupTextures();
	setupVBOs();
	setupRBOs();
	setupFBOs();

	//Fire up glut!
    glutMainLoop();
}

