#include"RBO.h"
#include"../graphics.h"

#include<stdlib.h>
#include<stdbool.h>

static RBO* boundRBO =  NULL;

static RBO* new() {
	RBO* rbo = malloc(sizeof(RBO));

	glGenRenderbuffers(1, &(rbo->id));
	rbo->height = 0;
	rbo->width = 0;

	return rbo;
}

static bool bind(RBO* rbo) {
	if (boundRBO!=NULL) {
		if (rbo->id == boundRBO->id) {
			return false;
		}
	}

	glBindRenderbuffer(GL_RENDERBUFFER, rbo->id);
	boundRBO = rbo;

	return true;
}

static bool unbind(RBO* rbo) {
	if (boundRBO!=NULL) {
		if (rbo->id == boundRBO->id) {
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			boundRBO = NULL;

			return true;
		}
	}

	return false;
}

static bool setStorage(RBO* rbo, GLenum type, uint32_t width, uint32_t height){
	if (bind(rbo)) {
		rbo->width = width;
		rbo->height = height;

		glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);

		unbind(rbo);
		return true;
	} else {
		return false;
	}
}

static void delete(RBO* rbo) {
	unbind(rbo);
	glDeleteFramebuffers(1, &(rbo->id));
	free(rbo);
}

const RBOManager manRBO = {new, bind, unbind, setStorage, delete};
