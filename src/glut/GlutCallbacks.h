
#ifndef SRC_GLUT_GLUTCALLBACKS_H_
#define SRC_GLUT_GLUTCALLBACKS_H_

/**
 * Contains the pointers to the main glut callbacks.
 * They may be null.
 */
struct GlutMainCallbacks_s {
	void(* onUpdate)();
	void(* onRender)();
};

/**
 * Contains the pointers to the main window callbacks.
 * They may be null.
 */
struct GlutWindowCallbacks_s {
	void(* onResize)(int , int);
};

/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct GlutMainCallbacks_s GlutMainCallbacks;
typedef struct GlutWindowCallbacks_s GlutWindowCallbacks;

#endif /* SRC_GLUT_GLUTCALLBACKS_H_ */
