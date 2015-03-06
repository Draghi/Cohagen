#ifndef SRC_GLUT_MOUSE_H_
#define SRC_GLUT_MOUSE_H_

#include <stdbool.h>

struct Mouse_s {
	void(*init)();
	void(*update)();

	bool(*isButtonDown)(int);

	int32_t(*getScrollUp)();
	int32_t(*getScrollDown)();
	int32_t(*getScrollCount)();

	int32_t(*getXPos)();
	int32_t(*getYPos)();

	int32_t(*getXMove)();
	int32_t(*getYMove)();
};

typedef struct Mouse_s Mouse;

extern const Mouse mouse;

#endif /* SRC_GLUT_MOUSE_H_ */
