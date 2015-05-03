#include "Display.h"

#include <stdlib.h>
#include <stdbool.h>

#include "lib/ogl.h"
#include "lib/glfw3.h"

#include "Mouse.h"

static bool isOpen(Window* window) {
	if (window->window != NULL)
		return !glfwWindowShouldClose(window->window);

	return false;
}

static bool makeContextCurrent(Window* window) {
	if (isOpen(window))
		glfwMakeContextCurrent(window->window);

	return window->window != NULL;
}

static Window* new() {
	Window* res = malloc(sizeof(Window));

	res->window = NULL;
	res->x = 0;
	res->y = 0;
	res->width = 0;
	res->height = 0;
	res->title = "";

	res->mouse = manMouse.new();

	return res;
}

static void delete(Window* window) {
	manMouse.delete(window->mouse);
	glfwDestroyWindow(window->window);
	free(window);
}

static bool openWindow(Window* window) {
	if (!window->window) {
		glfwWindowHint(GLFW_RESIZABLE, 0);
		glfwWindowHint(GLFW_VISIBLE, 1);
		glfwWindowHint(GLFW_DECORATED, 1);
		glfwWindowHint(GLFW_FOCUSED, 1);

		glfwWindowHint(GLFW_RED_BITS,     8);
		glfwWindowHint(GLFW_GREEN_BITS,   8);
		glfwWindowHint(GLFW_BLUE_BITS,    8);
		glfwWindowHint(GLFW_ALPHA_BITS,   8);
		glfwWindowHint(GLFW_DEPTH_BITS,   32);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_STEREO, 0);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_DOUBLEBUFFER, 1);

		glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
		glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);

		window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);

		if (window->window) {
			glfwSetWindowPos(window->window, window->x, window->y);
			glfwSetInputMode(window->window, GLFW_STICKY_KEYS, 1);
			makeContextCurrent(window);
		}

		glfwDefaultWindowHints();
	}

	return window->window!=NULL;
}

static void update(Window* window) {
	if(isOpen(window)) {
		manMouse.update(window);
		glfwPollEvents();
		//kb.update();

		glfwGetWindowPos(window->window, &window->x, &window->y);
		glfwGetWindowSize(window->window, &window->width, &window->height);
	}
}

static void swapBuffers(Window* window) {
	if(isOpen(window)) {
		glfwSwapBuffers(window->window);
	}
}

static void close(Window* window) {
	glfwSetWindowShouldClose(window->window, 1);
}

static void getDisplaySize(Window* window, int* size) {
	if (isOpen(window))
		glfwGetFramebufferSize(window->window, &size[0], &size[1]);
}

static void getMonitorSize(Window* window, int* size) {
	if (isOpen(window)) {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		size[0] = mode->width;
		size[1] = mode->height;
	}
}

static void setPosition(Window* window, int x, int y) {
	window->x = x;
	window->y = y;

	if (window->window)
		glfwSetWindowPos(window->window, x, y);
}

static void setSize(Window* window, int width, int height) {
	window->width = width;
	window->height = height;

	if (window->window!=NULL)
		glfwSetWindowSize(window->window, width, height);
}

static void setTitle(Window* window, char* title) {
	window->title = title;

	if (window->window!=NULL)
		glfwSetWindowTitle(window->window, title);
}

static int getDisplayWidth(Window* window) {
	int size[2];
	getDisplaySize(window, size);
	return size[0];
}

static int getDisplayHeight(Window* window) {
	int size[2];
	getDisplaySize(window, size);
	return size[1];
}

static int getMontiorWidth(Window* window) {
	int size[2];
	getMonitorSize(window, size);
	return size[0];
}

static int getMonitorHeight(Window* window) {
	int size[2];
	getMonitorSize(window, size);
	return size[1];
}

static void getSize(Window* window, int* size) {
	size[0] = window->x;
	size[1] = window->y;
}

static void getPos(Window* window, int* pos) {
	pos[0] = window->width;
	pos[1] = window->height;
}

static int getX(Window* window) {
	return window->x;
}

static int getY(Window* window) {
	return window->x;
}

static int getWidth(Window* window) {
	return window->width;
}

static int getHeight(Window* window) {
	return window->height;
}

static void centerWindow(Window* window) {
	int x = (getMontiorWidth(window)-getWidth(window))/2;
	int y = (getMonitorHeight(window)-getHeight(window))/2;
	setPosition(window, x, y);
}

const WindowManager manWin = {isOpen,makeContextCurrent,new,delete,openWindow,update,swapBuffers,close,getDisplaySize,getMonitorSize,setPosition,setSize,setTitle,getDisplayWidth,getDisplayHeight,getMontiorWidth,getMonitorHeight,getSize,getPos,getX,getY,getWidth,getHeight,centerWindow};
