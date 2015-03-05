
#ifndef SRC_GLUT_GLUTCALLBACKS_H_
#define SRC_GLUT_GLUTCALLBACKS_H_

struct GlutMainCallbacks_s {
	void(* onUpdate)();
	void(* onRender)();
};

struct GlutWindowCallbacks_s {
	void(* onResize)(int , int);
	//void(* onReposition)(int, int); Can't find a call back for this
};

typedef struct GlutMainCallbacks_s GlutMainCallbacks;
typedef struct GlutWindowCallbacks_s GlutWindowCallbacks;

#endif /* SRC_GLUT_GLUTCALLBACKS_H_ */
