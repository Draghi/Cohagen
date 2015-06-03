#include <stdio.h>
#include <string.h>

#include "Skybox.h"

static VAO *genSkyboxQuad()
{
	const int quadCoordSize = 12;

	float quadCoords[12] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f,
        -1.0f, -1.0f
	};

	VBO *vbo = manVBO.new();
	manVBO.setData(vbo, quadCoords, sizeof(float) * quadCoordSize, GL_STATIC_DRAW);
	manVBO.setRenderInfo(vbo, quadCoordSize, 2, 0, NULL);

	VAO *vao = manVAO.new();
	// quadCoordSize/2 is the number of verts to draw (2 floats per vertex)
	manVAO.setRenderInfo(vao, quadCoordSize/2);
	manVAO.attachVBO(vao, vbo, 0, GL_FLOAT);

	free(vbo);

	return vao;
}

static Texture *genSkyboxTexture(	const char *const front, const char *back,
									const char *const top, const char *const bottom,
									const char *const left, const char *const right)
{
	Texture 	*tex;
	FileData 	fFront, fBack, fTop, fBottom, fRight, fLeft;
	Bitmap 		*bmpFront, *bmpBack, *bmpTop, *bmpBottom, *bmpLeft, *bmpRight;

	tex = manTex.new();

	if (fileUtil.loadFile(front, &fFront) != FILE_SUCCEEDED) {
		printf("Failed to open skybox texture %s\n", front);
	} else if (fileUtil.loadFile(back, &fBack) != FILE_SUCCEEDED) {
		printf("Failed to open skybox texture %s\n", back);
	} else if (fileUtil.loadFile(top, &fTop) != FILE_SUCCEEDED) {
		printf("Failed to open skybox texture %s\n", top);
	} else if (fileUtil.loadFile(bottom, &fBottom) != FILE_SUCCEEDED) {
		printf("Failed to open skybox texture %s\n", bottom);
	} else if (fileUtil.loadFile(left, &fLeft) != FILE_SUCCEEDED) {
		printf("Failed to open skybox texture %s\n", left);
	} else if (fileUtil.loadFile(right, &fRight) != FILE_SUCCEEDED) {
		printf("Failed to open skybox texture %s\n", right);
	} else {			
		bmpFront = manBitmap.new(fFront.data, fFront.size);
		bmpBack = manBitmap.new(fBack.data, fBack.size);
		bmpTop = manBitmap.new(fTop.data, fTop.size);
		bmpBottom = manBitmap.new(fBottom.data, fBottom.size);
		bmpLeft = manBitmap.new(fLeft.data, fLeft.size);
		bmpRight = manBitmap.new(fRight.data, fRight.size);

		if (manTex.bind(tex, GL_TEXTURE_CUBE_MAP, 0) != false) {
			manTex.imageToTarget(tex, 0, (uint8_t *)bmpRight->pixels, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, bmpRight->width, bmpRight->height);
			manTex.imageToTarget(tex, 0, (uint8_t *)bmpLeft->pixels,  GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, bmpLeft->width, bmpLeft->height);
			manTex.imageToTarget(tex, 0, (uint8_t *)bmpTop->pixels,   GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, bmpTop->width, bmpTop->height);
			manTex.imageToTarget(tex, 0, (uint8_t *)bmpBottom->pixels,GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, bmpBottom->width, bmpBottom->height);
			manTex.imageToTarget(tex, 0, (uint8_t *)bmpFront->pixels, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, bmpFront->width, bmpFront->height);
			manTex.imageToTarget(tex, 0, (uint8_t *)bmpBack->pixels,  GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, bmpBack->width, bmpBack->height);

			manTex.formatTexture(tex, 0, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			manTex.formatTexture(tex, 0, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			manTex.formatTexture(tex, 0, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
			manTex.formatTexture(tex, 0, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
			manTex.formatTexture(tex, 0, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			manTex.formatTexture(tex, 0, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			manTex.formatTexture(tex, 0, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			manTex.unbind(tex, GL_TEXTURE_CUBE_MAP);
		} else {
			manTex.delete(tex);
			free(tex);
			tex = NULL;
		}

		manBitmap.delete(bmpFront);
		manBitmap.delete(bmpBack);
		manBitmap.delete(bmpTop);
		manBitmap.delete(bmpBottom);
		manBitmap.delete(bmpLeft);
		manBitmap.delete(bmpRight);

		if (fFront.data != NULL) {
			free(fFront.data);
		}
		if (fBack.data != NULL) {
			free(fBack.data);
		}
		if (fTop.data != NULL) {
			free(fTop.data);
		}
		if (fBottom.data != NULL) {
			free(fBottom.data);
		}
		if (fLeft.data != NULL) {
			free(fLeft.data);
		}
		if (fRight.data != NULL) {
			free(fRight.data);
		}
	}

	return tex;
}

static Skybox *new( const char *const front, const char *const back,
					const char *const top, const char *const bottom,
					const char *const left, const char *const right) {
	Skybox *skybox = malloc(sizeof(Skybox));

	skybox->vao = genSkyboxQuad();
	skybox->tex = genSkyboxTexture(front, back, top, bottom, left, right);

	return skybox;
}

static Skybox *newFromGroup(const char *const path, const char *const baseName) {
	unsigned int baseStrLen = strlen(baseName);
	unsigned int pathStrLen = strlen(path);

	// +1 for NULL terminating character
	char *frontStr = malloc(sizeof(char) * (baseStrLen + pathStrLen + sizeof("_front.bmp") + 1));
	sprintf(frontStr, "%s%s_front.bmp", path, baseName);

	char *backStr = malloc(sizeof(char) * (baseStrLen + pathStrLen + sizeof("_back.bmp") + 1));
	sprintf(backStr, "%s%s_back.bmp", path, baseName);

	char *leftStr = malloc(sizeof(char) * (baseStrLen + pathStrLen + sizeof("_left.bmp") + 1));
	sprintf(leftStr, "%s%s_left.bmp", path, baseName);

	char *rightStr = malloc(sizeof(char) * (baseStrLen + pathStrLen + sizeof("_right.bmp") + 1));
	sprintf(rightStr, "%s%s_right.bmp", path, baseName);

	char *topStr = malloc(sizeof(char) * (baseStrLen + pathStrLen + sizeof("_top.bmp") + 1));
	sprintf(topStr, "%s%s_top.bmp", path, baseName);

	char *bottomStr = malloc(sizeof(char) * (baseStrLen + pathStrLen + sizeof("_bottom.bmp") + 1));
	sprintf(bottomStr, "%s%s_bottom.bmp", path, baseName);

	Skybox *skybox = new(frontStr, backStr, topStr, bottomStr, leftStr, rightStr);

	free(frontStr);
	free(backStr);
	free(leftStr);
	free(rightStr);
	free(topStr);
	free(bottomStr);

	return skybox;
}

static void draw(const Skybox *const skybox, const Shader *const shader, const char *const cubemapShaderName) {
	glDepthMask(GL_FALSE);

	manShader.bindUniformFloat(shader, cubemapShaderName, skybox->tex->slotID);	

	manShader.bind(shader);
		manVAO.bind(skybox->vao);
			manTex.bind(skybox->tex, GL_TEXTURE_CUBE_MAP, 0);
				manVAO.draw(skybox->vao);
			manTex.unbind(skybox->tex, GL_TEXTURE_CUBE_MAP);
		manVAO.unbind();
	manShader.unbind();
	
	glDepthMask(GL_TRUE);
}

const SkyboxManager manSkybox = {new, newFromGroup, draw};
