#include"FBO.h"

#include"../lib/ogl.h"
#include"RBO.h"
#include"../util/DynamicArray.h"

#include<stdlib.h>
#include<stdbool.h>

static FBO* boundFBO =  NULL;

static FBO* new() {
	FBO* fbo = malloc(sizeof(FBO));

	glGenFramebuffers(1, &(fbo->id));
	fbo->height = 0;
	fbo->width = 0;
	fbo->textures = malloc(sizeof(DynamicArray));
	fbo->texRelations = malloc(sizeof(DynamicArray));
	fbo->renderbuffers = malloc(sizeof(DynamicArray));

	newDynamicArray(fbo->textures, 1, sizeof(void*));
	newDynamicArray(fbo->renderbuffers, 1, sizeof(void*));
	newDynamicArray(fbo->texRelations, 1, sizeof(void*));

	return fbo;
}

static bool bind(FBO* fbo) {
	if (boundFBO!=NULL) {
		if (fbo->id == boundFBO->id) {
			return false;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo->id);
	boundFBO = fbo;

	return true;
}

static bool unbind(FBO* fbo) {
	if (boundFBO!=NULL) {
		if (fbo->id == boundFBO->id) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			boundFBO = NULL;

			return true;
		}
	}

	return false;
}

static bool attachTexture(FBO* fbo, Texture* texture, GLenum relation) {
	if (bind(fbo)) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, relation, GL_TEXTURE_2D, texture->id, 0);
		fbo->textures->append(fbo->textures, texture);

		GLenum *heapRel = malloc(sizeof(GLenum));
		*heapRel = relation;
		fbo->texRelations->append(fbo->texRelations, heapRel);

		glDrawBuffers(fbo->texRelations->size, *((GLenum**)(fbo->texRelations->contents)));

		unbind(fbo);
		return true;
	} else {
		return false;
	}
}

static bool attachRBO(FBO* fbo, RBO* rbo, GLenum relation) {
	if (bind(fbo)) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, relation, GL_RENDERBUFFER, rbo->id);
		fbo->renderbuffers->append(fbo->renderbuffers, rbo);
		unbind(fbo);

		return true;
	} else {
		return false;
	}
}

static void delete(FBO* fbo) {
	unbind(fbo);
	glDeleteFramebuffers(1, &(fbo->id));
	free(fbo);
}

const FBOManager manFBO = {new, bind, unbind, attachTexture, attachRBO, delete};
