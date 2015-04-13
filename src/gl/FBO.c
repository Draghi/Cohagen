#include"FBO.h"

#include <stdlib.h>
#include <stdbool.h>

#include "lib/ogl.h"
#include "gl/RBO.h"
#include "util/DynamicArray.h"

static FBO* boundFBO =  NULL;

static FBO* new() {
	FBO* fbo = malloc(sizeof(FBO));

	glGenFramebuffers(1, &(fbo->id));
	fbo->height = 0;
	fbo->width = 0;

	fbo->textures = manDynamicArray.new(1, sizeof(void*));
	fbo->renderbuffers = manDynamicArray.new(1, sizeof(void*));
	fbo->texRelations = manDynamicArray.new(1, sizeof(void*));

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
		manDynamicArray.append(fbo->textures, texture);

		GLenum *heapRel = malloc(sizeof(GLenum));
		*heapRel = relation;
		manDynamicArray.append(fbo->texRelations, heapRel);

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
		manDynamicArray.append(fbo->renderbuffers, rbo);
		unbind(fbo);

		return true;
	} else {
		return false;
	}
}

static void delete(FBO* fbo) {
	unbind(fbo);
	glDeleteFramebuffers(1, &(fbo->id));

	// Use of these free statements may crash the program if you at any point
	// put stack memory inside the array. Remove these statements if you do.
	manDynamicArray.free(fbo->textures);
	manDynamicArray.free(fbo->texRelations);
	manDynamicArray.free(fbo->renderbuffers);

	manDynamicArray.delete(fbo->textures);
	manDynamicArray.delete(fbo->texRelations);
	manDynamicArray.delete(fbo->renderbuffers);

	free(fbo);
}

const FBOManager manFBO = {new, bind, unbind, attachTexture, attachRBO, delete};
