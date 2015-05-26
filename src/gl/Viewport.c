#include "Viewport.h"

#include "lib/ogl.h"
#include "util/OGLUtil.h"

static void makeActive(const Viewport *const viewport);
static float GetX();
static void SetX(float newX);
static float GetY();
static void SetY(float newY);
static float GetHeight();
static void SetHeight(float newHeight);
static float GetWidth();
static void SetWidth(float newWidth);


static void makeActive(const Viewport *const viewport){
	glViewport(viewport->x, viewport->y, viewport->width, viewport->height);
}


static float GetX(const Viewport *const viewport){
	return viewport->x;
}


static void SetX(const Viewport *const viewport, float newX){
	viewport->x = newX;
}


static float GetY(const Viewport *const viewport){
	return viewport->y;
}


static void SetY(const Viewport *const viewport, float newY){
	viewport->y = newY;
}


static float GetHeight(const Viewport *const viewport){
	return viewport->height;
}


static void SetHeight(const Viewport *const viewport, float newHeight){
	viewport->height = newHeight;
}


static float GetWidth(const Viewport *const viewport){
	return viewport->width;
}


static void SetWidth(const Viewport *const viewport, float newWidth){
	viewport->width = newWidth;
}

