#ifndef COH_RENDERER_H
#define COH_RENDERER_H

#include "render/RenderObject.h"
#include "render/MatrixManager.h"
#include "gl/Shader.h"

static const char PROJECTION_MATRIX_NAME[] = "projMat";
static const char       VIEW_MATRIX_NAME[] = "viewMat";
static const char      MODEL_MATRIX_NAME[] = "modelMat";
static const char        MVP_MATRIX_NAME[] = "mvpMat";

typedef struct RendererManager_s {
	void(* applyTransformations)(RenderObject* model, MatrixManager* matMan);
	void(* bindTextures)(DynamicArray* textures);
	void(* bindMatricies)(Shader* shader, MatrixManager* matMan);
	void(* renderModel)(RenderObject* model, Shader* shader, MatrixManager* matMan);
} RendererManager;

extern const RendererManager manRenderer;

#endif
