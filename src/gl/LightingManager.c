#include "LightingManager.h"
#include "Light.h"

static LightingManager *newDefault() {
	LightingManager *lightingManager = malloc(sizeof(LightingManager));

	lightingManager->ambientIntensity = manVec4.create(NULL, 1.0f, 1.0f, 1.0f, 1.0f);
	lightingManager->lightAttenuation = 1.5f;
	lightingManager->maxIntensity = 1.0f;
	lightingManager->gamma = 2.2f;

	for (int i = 0; i < MAX_NUMBER_OF_LIGHTS; ++i) {
		lightingManager->lights[i] = manLight.newDefault();
	}

	return lightingManager;
}

static LightingManager *new(const Vec4 *const ambientIntensity, float lightAttenuation, float maxIntensity, float gamma) {
	LightingManager *lightingManager = malloc(sizeof(LightingManager));

	lightingManager->ambientIntensity = *ambientIntensity;
	lightingManager->lightAttenuation = lightAttenuation;
	lightingManager->maxIntensity = maxIntensity;
	lightingManager->gamma = gamma;

	for (int i = 0; i < MAX_NUMBER_OF_LIGHTS; ++i) {
		lightingManager->lights[i] = manLight.newDefault();
	}

	return lightingManager;
}

static void delete(const LightingManager *const lightingManager) {
	for (int i = 0; i < MAX_NUMBER_OF_LIGHTS; ++i) {
		manLight.delete(lightingManager->lights[i]);
		free(lightingManager->lights[i]);
	}
}

static Vec4 getAmbientIntensity(const LightingManager *const lightingManager) {
	return lightingManager->ambientIntensity;
}

static void setAmbientIntensity(LightingManager *const lightingManager, const Vec4 *const ambientIntensity) {
	lightingManager->ambientIntensity = *ambientIntensity;
}

static float getAttenuation(const LightingManager *const lightingManager) {
	return lightingManager->lightAttenuation;
}

static void setAttenuation(LightingManager *const lightingManager, float attenuation) {
	lightingManager->lightAttenuation = attenuation;
}

static float getMaxLightIntensity(const LightingManager *const lightingManager) {
	return lightingManager->maxIntensity;
}

static void setMaxLightIntensity(LightingManager *const lightingManager, float maxIntensity) {
	lightingManager->maxIntensity = maxIntensity;
}

static float getGamma(const LightingManager *const lightingManager) {
	return lightingManager->gamma;
}

static void setGamma(LightingManager *const lightingManager, float gamma) {
	lightingManager->gamma = gamma;
}

static void populateLightBlock(LightingManager *const lightingManager) {
	lightingManager->lightData.ambientIntensity = lightingManager->ambientIntensity;
	lightingManager->lightData.lightAttenuation = lightingManager->lightAttenuation;
	lightingManager->lightData.maxIntensity = lightingManager->maxIntensity;
	lightingManager->lightData.invGamma = 1.0f / lightingManager->gamma;

	for (int i = 0; i < MAX_NUMBER_OF_LIGHTS; ++i) {
		lightingManager->lightData.lights[i].lightWorldPos = lightingManager->lights[i]->pos;
		lightingManager->lightData.lights[i].lightIntensity = manVec4.createFromVec3(NULL, &(lightingManager->lights[i]->intensity), 1.0f);
		lightingManager->lightData.lights[i].isOn = lightingManager->lights[i]->isOn ? 1 : 0;
	}
}

static void update(LightingManager *const lightingManager) {
	populateLightBlock(lightingManager);
}

const LightingManagerManager manLightingManager = {newDefault, new, delete, getAmbientIntensity, setAmbientIntensity, getAttenuation, setAttenuation, getMaxLightIntensity, setMaxLightIntensity, getGamma, setGamma, update};
