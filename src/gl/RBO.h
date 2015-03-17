
#ifndef COH_RBO_H
#define COH_RBO_H

#include"../graphics.h"

struct RBO_s {
	GLuint id;
	GLuint width;
	GLuint height;
};

typedef struct RBO_s RBO;

struct RBOManager_s {
	RBO*(* new)();
	bool(* bind)(RBO*);
	bool(* unbind)(RBO*);
	bool(* setStorage)(RBO*, GLenum, GLenum, uint32_t);
	void(* delete)(RBO*);
};

typedef struct RBOManager_s RBOManager;

extern const RBOManager manRBO;

#endif
