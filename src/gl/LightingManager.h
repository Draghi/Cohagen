#ifndef COH_LIGHTING_MANAGER
#define COH_LIGHTING_MANAGER

#include "Light.h"

#define MAX_NUMBER_OF_LIGHTS 8

typedef struct PerLight_s {
	Vec4 			lightWorldPos;
	Vec4 			lightIntensity;
	unsigned int 	isOn;
	float 			padding[3];
} PerLight;

typedef struct LightBlock_s {
	Vec4 		ambientIntensity;
	float 		lightAttenuation;
	float 		maxIntensity;
	float 		invGamma;
	float 		padding;
	PerLight 	lights[MAX_NUMBER_OF_LIGHTS];
} LightBlock;

typedef struct LightingManager_s {
	Light 	*lights[MAX_NUMBER_OF_LIGHTS];
	Vec4 	ambientIntensity;
	float 	lightAttenuation;
	float 	maxIntensity;
	float 	gamma;
	LightBlock 	lightData;
} LightingManager;

typedef struct LightingManagerManager_s {
	/**
	 * 	Create a lighting manager with the following default values:
	 * 		ambientIntensity = 1.0f, 1.0f, 1.0f, 1.0f
	 * 		lightAttenuation = 1.5f;
	 * 		maxIntensity = 1.5f
	 * 		gamma = 2.2f
	 *
	 * 	@return 	pointer to LightingManager, lighting manager object created.
	 */
	LightingManager *(*newDefault)();

	/**
	 * Create a lighting manager with the given values.
	 *
	 * 	@param 	ambientIntensity 	const pointer to const Vec4, ambient intensity of light in the scene.
	 * 	@param 	lightAttenuation 	float, attenuation of light in the scene.
	 * 	@param 	maxIntensity 		float, maximum intensity of any lights (or combination of lights) in the scene.
	 * 	@param 	gamma 				float, gamma correction value.
	 *
	 * 	@return 					pointer to LightingManager, lighting manager object.
	 */
	LightingManager *(*new)(const Vec4 *const ambientIntensity, float lightAttenuation, float maxIntensity, float gamma);

	/**
	 * 	Delete all memory associated with the lighting manager but not the lighting manager itself.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager to delete.
	 */
	void (*delete)(const LightingManager *const lightingManager);

	/**
	 *	Get the ambient intensity of light in the scene.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to get ambient intensity of.
	 * 	@return 					Vec4, ambient intensity of light in the scene.
	 */
	Vec4 (*getAmbientIntensity)(const LightingManager *const lightingManager);

	/**
	 *	Set the ambient intensity of light in the scene.
	 * 	
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to set ambient intensity of.
	 * 	@param 	ambientIntensity 	const pointer to const Vec4, ambient intensity value to set.
	 */
	void (*setAmbientIntensity)(LightingManager *const lightingManager, const Vec4 *const ambientIntensity);

	/**
	 *	Get the attenuation of light in the scene.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to get attenuation of.
	 * 	@return 					float, attenuation of light in the scene.
	 */
	float (*getAttenuation)(const LightingManager *const lightingManager);

	/**
	 *	Set the attenuation of light in the scene.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to set attenuation of.
	 * 	@param 	attenuation 		float, attenuation to set to.
	 */
	void (*setAttenuation)(LightingManager *const lightingManager, float attenuation);

	/**
	 *	Get the maximum light intensity of the scene.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to get max light intensity of.
	 *	@return 					float, maximum light intensity of the scene.
	 */
	float (*getMaxLightIntensity)(const LightingManager *const lightingManager);

	/**
	 *	Set the maximum light intensity of the scene.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to set max light intensity of.
	 * 	@param 	maxIntensity 		float, maximum intensity of light in the scene.s
	 */
	void (*setMaxLightIntensity)(LightingManager *const lightingManager, float maxIntensity);

	/**
	 *	Get the gamma value.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to get the gamma of.
	 * 	@return 					float, gamma value.
	 */
	float (*getGamma)(const LightingManager *const lightingManager);

	/**
	 *	Set the gamma value.
	 *
	 * 	@param 	lightingManager 	const pointer to const LightingManager, lighting manager to set gamma of.
	 * 	@param	gamma 				float, gamma value to set.
	 */
	void (*setGamma)(LightingManager *const lightingManager, float gamma);

	// void (*populateLightBlock)(LightingManager *const lightingManager);

	void (*update)(LightingManager *const lightingManager);
} LightingManagerManager;

extern const LightingManagerManager manLightingManager;

#endif //COH_LIGHTING_MANAGER
