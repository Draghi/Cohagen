
#include "../graphics.h"
#include <stdbool.h>
#include <stdio.h>

bool glCheckError() {
	bool flag = false;
	while(true) {
		GLenum glErr = glGetError();

		switch(glErr) {
			case GL_NO_ERROR:
				if (!flag)
					glLog.logGLError("GLError(%d) No error\n", glErr);
				return flag;

			case GL_INVALID_ENUM:
				glLog.logGLError("GLError(%d) Invalid Enum\n", glErr);
				flag = true;
				break;

			case GL_INVALID_VALUE:
				glLog.logGLError("GLError(%d) Invalid Value\n", glErr);
				flag = true;
				break;

			case GL_INVALID_OPERATION:
				glLog.logGLError("GLError(%d) Invalid Operation\n", glErr);
				flag = true;
				break;

			case GL_STACK_OVERFLOW:
				glLog.logGLError("GLError(%d) Stack Overflow\n", glErr);
				flag = true;
				break;

			case GL_STACK_UNDERFLOW:
				glLog.logGLError("GLError(%d) Stack Underflow\n", glErr);
				flag = true;
				break;

			case GL_OUT_OF_MEMORY:
				glLog.logGLError("GLError(%d) Out of memory\n", glErr);
				flag = true;
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				glLog.logGLError("GLError(%d) Invalid Framebuffer operation\n", glErr);
				flag = true;
				break;

			default:
				glLog.logGLError("GLError(%d) Unknown error occurred\n", glErr);
				flag = true;
				break;
		}
	}
}
