
#include "../graphics.h"
#include <stdbool.h>
#include <stdio.h>

bool checkGLError() {
	bool flag = false;
	while(true) {
		GLenum glErr = glGetError();

		switch(glErr) {
			case GL_NO_ERROR:
				if (!flag)
					printf("GLError(%d) No error\n", glErr);
				return flag;

			case GL_INVALID_ENUM:
				printf("GLError(%d) Invalid Enum\n", glErr);
				flag = true;
				break;

			case GL_INVALID_VALUE:
				printf("GLError(%d) Invalid Value\n", glErr);
				flag = true;
				break;

			case GL_INVALID_OPERATION:
				printf("GLError(%d) Invalid Operation\n", glErr);
				flag = true;
				break;

			case GL_STACK_OVERFLOW:
				printf("GLError(%d) Stack Overflow\n", glErr);
				flag = true;
				break;

			case GL_STACK_UNDERFLOW:
				printf("GLError(%d) Stack Underflow\n", glErr);
				flag = true;
				break;

			case GL_OUT_OF_MEMORY:
				printf("GLError(%d) Out of memory\n", glErr);
				flag = true;
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				printf("GLError(%d) Invalid Framebuffer operation\n", glErr);
				flag = true;
				break;

			default:
				printf("GLError(%d) Unknown error occurred\n", glErr);
				flag = true;
				break;
		}
	}
}
