#include "Renderer.h"

static const Vec3 xAxis = {1, 0, 0};
static const Vec3 yAxis = {0, 1, 0};
static const Vec3 zAxis = {0, 0, 1};

static void applyTransformations(RenderObject* model, MatrixManager* matMan) {
	manMatMan.scale(matMan, *model->scale);
	manMatMan.rotate(matMan, model->rotation->x, xAxis);
	manMatMan.rotate(matMan, model->rotation->y, yAxis);
	manMatMan.rotate(matMan, model->rotation->z, zAxis);
	manMatMan.translate(matMan, *model->position);
}

static void bindTextures(DynamicArray* textures) {
	if (textures!=NULL) {
		for(unsigned int i = 0; i < textures->size; i++) {
			manTex.bind((Texture*)manDynamicArray.get(textures, i), GL_TEXTURE_2D, i);
		}
	}
}

static void bindMatricies(Shader* shader, MatrixManager* matMan) {
	if ((matMan!=NULL) && (shader!=NULL)) {
		Mat4* projMat = manMatMan.peekStack(matMan, MATRIX_MODE_PROJECTION);
		Mat4* viewMat = manMatMan.peekStack(matMan, MATRIX_MODE_VIEW);
		Mat4* modelMat = manMatMan.peekStack(matMan, MATRIX_MODE_MODEL);
		Mat4 mvpMat = manMat4.mul(projMat, viewMat);
		mvpMat = manMat4.mul(&mvpMat, modelMat);

		manShader.bindUniformMat4(shader, PROJECTION_MATRIX_NAME, projMat);
		manShader.bindUniformMat4(shader, VIEW_MATRIX_NAME, viewMat);
		manShader.bindUniformMat4(shader, MODEL_MATRIX_NAME, projMat);
		manShader.bindUniformMat4(shader, MVP_MATRIX_NAME, &mvpMat);
	}
}

static void renderModel(RenderObject* model, Shader* shader, MatrixManager* matMan) {
	if (model != NULL) {
		if (model->model != NULL) {
			manMatMan.push(matMan);
				applyTransformations(model, matMan);
				manShader.bind(shader);
					bindMatricies(shader, matMan);
					bindTextures(model->textures);
					manVAO.draw(model->model);
				manShader.unbind(shader);
			manMatMan.pop(matMan);
		}
	}
}

const RendererManager manRenderer = {applyTransformations, bindTextures, bindMatricies, renderModel};
