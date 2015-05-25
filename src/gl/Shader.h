#ifndef COH_SHADER_H
#define COH_SHADER_H

#include <stdarg.h>

#include "lib/ogl.h"
#include "math/Mat4.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"

/**
 *  Shader object.
 */
typedef struct Shader_s {

	/**
     *  GL linked program object.
     */
    GLuint program;
} Shader;

/**
 *  Class to manage the use of Shader objects.
 */
typedef struct ShaderManager_s {

    /**
     *  Bind the shader program.
     *
     *  @param     shader  pointer to Shader object to bind.
     */
    void (*bind)(const Shader *const);

    /**
     *  Unbind the shader program.
     */
    void (*unbind)();

    /**
     *  Returns a pointer to a Shader object, constructed from all shaders
     *  matching the given baseFileName at the given path.
     *
     *  @param  path            const pointer to const char, path to shaders.
     *  @param  baseFileName    const pointer to const char, base file name of shaders.
     *  @returns                pointer to Shader object.
     */
    Shader *(*newFromGroup)(const char *const path, const char *const baseFileName);

    /**
     *  Bind a Mat4 to a uniform in the given shader.
     *
     *  @pre    Appropriate Shader must be bound before method invocation.
     *
     *  @param  shader          const pointer to const Shader, shader to find uniform in.
     *  @param  uniformName     pointer to const char, C-style string, name of uniform in shader to bind to.
     *  @param  matrix          const pointer to const Mat4, matrix to bind.
     *  @returns                int, return value of "glGetUniformLocation", if -1, uniformName was not found.
     */
    int (*bindUniformMat4)(const Shader *const shader, const char *uniformName, const Mat4 *const matrix);

    /**
     *  Bind an integer value to a uniform in the given shader.
     *
     *  @pre    Appropriate Shader must be bound before method invocation.
     *
     *  @param  shader          const pointer to const Shader, shader to find uniform in.
     *  @param  uniformName     pointer to const char, C-style string, name of uniform in shader to bind to.
     *  @param  intToBind       int, integer value to bind to the uniform in the shader.
     *  @returns                int, return value of "glGetUniformLocation", if -1, uniformName was not found.
     */
    int (*bindUniformInt)(const Shader *const shader, const char *uniformName, int intToBind);

    /**
     *  Bind a float value to a uniform in the given shader.
     *
     *  @pre    Appropriate Shader must be bound before method invocation.
     *
     *  @param  shader          const pointer to const Shader, shader to find uniform in.
     *  @param  uniformName     pointer to const char, C-style string, name of uniform in shader to bind to.
     *  @param  floatToBind     float, float value to bind to the uniform in the shader.
     *  @returns                int, return value of "glGetUniformLocation", if -1, uniformName was not found.
     */
    int (*bindUniformFloat)(const Shader *const shader, const char *uniformName, float floatToBind);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param scal				Specifies the new value to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformScalar)(GLint location, const scalar scal);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param vec				Specifies the new values to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformVec2)(GLint location, const Vec2 const* vec);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param vec				Specifies the new values to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformVec3)(GLint location, const Vec3 const* vec);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param vec				Specifies the new values to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformVec4)(GLint location, const Vec4 const* vec);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0				Specifies the new value to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformInt1)(GLint location, GLint v0);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0, v1			Specifies the new values to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformInt2)(GLint location, GLint v0, GLint v1);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0, v1, v2		Specifies the new values to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformInt3)(GLint location, GLint v0, GLint v1, GLint v2);

    /**
     * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0, v1, v2, v3	Specifies the new values to be used for the specified uniform variable.
     * @returns void
     */
    void (*bindUniformInt4)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

    /**
	 * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0				Specifies the new value to be used for the specified uniform variable.
     * @returns void
	 */
	void (*bindUniformUInt1)(GLint location, GLuint v0);

	/**
	 * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0, v1			Specifies the new values to be used for the specified uniform variable.
     * @returns void
	 */
	void (*bindUniformUInt2)(GLint location, GLuint v0, GLuint v1);

	/**
	 * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0, v1, v2		Specifies the new values to be used for the specified uniform variable.
     * @returns void
	 */
	void (*bindUniformUInt3)(GLint location, GLuint v0, GLuint v1, GLuint v2);

	/**
	 * Specify the value of a uniform variable for the current program object.
     *
     * @param location			Specifies the location of the uniform variable to be modified.
     * @param v0, v1, v2, v3	Specifies the new values to be used for the specified uniform variable.
     * @returns void
	 */
	void (*bindUniformUInt4)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

} ShaderManager;

extern const ShaderManager manShader;

#endif
