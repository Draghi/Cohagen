#include "Viewport.h"

#include <stdlib.h>
#include "lib/ogl.h"
#include "util/OGLUtil.h"

static void makeActive(const Viewport *const viewport);
static float getX(const Viewport *const viewport);
static void setX(Viewport *const viewport, float newX);
static float getY(const Viewport *const viewport);
static void setY(Viewport *const viewport, float newY);
static float getHeight(const Viewport *const viewport);
static void setHeight(Viewport *const viewport, float newHeight);
static float getWidth(const Viewport *const viewport);
static void setWidth(Viewport *const viewport, float newWidth);


static Viewport* new(float x, float y, float width, float height){
	Viewport* viewport = malloc(sizeof(Viewport));

	viewport->x = x;

	viewport->y = y;

	if(width > 0){
		viewport->width = width;
	} else {
		viewport->width = 100;
	}

	if(height > 0){
		viewport->height = height;
	} else {
		viewport->height = 100;
	}

	return viewport;
}

static void makeActive(const Viewport *const viewport) {
	glViewport(viewport->x, viewport->y, viewport->width, viewport->height);
}

static float getX(const Viewport *const viewport){
	return viewport->x;
}

static void setX(Viewport *const viewport, float newX){
	viewport->x = newX;
}

static float getY(const Viewport *const viewport){
	return viewport->y;
}

static void setY(Viewport *const viewport, float newY){
	viewport->y = newY;
}

static float getHeight(const Viewport *const viewport){
	return viewport->height;
}

static void setHeight(Viewport *const viewport, float newHeight){
	viewport->height = newHeight;
}

static float getWidth(const Viewport *const viewport){
	return viewport->width;
}

static void setWidth(Viewport *const viewport, float newWidth){
	viewport->width = newWidth;
}

static void delete(Viewport *const viewport){

}

const ViewportManager manViewport = {new, makeActive, getX, setX, getY, setY, getHeight, setHeight, getWidth, setWidth, delete};
