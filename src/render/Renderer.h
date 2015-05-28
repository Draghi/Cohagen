#ifndef COH_RENDERER_H
#define COH_RENDERER_H

#include "render/RenderObject.h"
#include "render/MatrixManager.h"
#include "gl/Shader.h"

/** The name of the projection matrix to look for in a shader **/
static const char PROJECTION_MATRIX_NAME[] = "projMat";
/** The name of the view matrix to look for in a shader. **/
static const char       VIEW_MATRIX_NAME[] = "viewMat";
/** The name of the model matrix to look for in a shader. **/
static const char      MODEL_MATRIX_NAME[] = "modelMat";
/** The name of the model-View-Projection matrix to look for in a shader. **/
static const char        MVP_MATRIX_NAME[] = "mvpMat";

typedef struct RendererManager_s {
	/**
	 * Applies the transformations contained in the RenderObject to the given MatrixManager
	 * @param model The model to use the transformations of.
	 * @param matMan The matrix manager to transform.
	 */
	void(* applyTransformations)(RenderObject* model, MatrixManager* matMan);
	/**
	 * Binds the textures in the given DynamicArray, in order.
	 * @remarks Will bind to texture slots 0..length(DynamicArray)-1 in the order of the array.
	 * @param textures The textures to bind.
	 */
	void(* bindTextures)(DynamicArray* textures);
	/**
	 * Binds the projection, model and view matrix to the given shader.
	 * Also calcuates the model-view-projection matrix and binds to the given shader.
	 * @param shader The shader to bind too.
	 * @param matMan The matrix manager to get the matrices from.
	 */
	void(* bindMatricies)(Shader* shader, MatrixManager* matMan);
	/**
	 * Renders the given render object, using the shader and matrix manager.
	 * @param model The render object to render.
	 * @param shader The shader to use.
	 * @param matMan The Matrix Manager to use.
	 */
	void(* renderModel)(RenderObject* model, Shader* shader, MatrixManager* matMan);
} RendererManager;

extern const RendererManager manRenderer;

#endif
