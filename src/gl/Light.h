#ifndef COH_LIGHT
#define COH_LIGHT

#include <stdbool.h>

#include "math/Vec4.h"

typedef struct Light_s {
	bool 	isOn;
	Vec3 	intensity;
	Vec4 	pos;
} Light;

typedef struct LightManager_s {
	/**
	 *	Constructs a light that's off,
	 * 	has an intensity of 0.0f, 0.0f, 0.0f, 
	 * 	and a position of 0, 0, 0, 0.0f.
	 * 	
	 * 	@returns 	pointer to Light, constructed Light object.
	 */
	Light *(*newDefault)();

	/**
	 *	Constructs a new Light object from the given parameters.
	 *
	 * 	@param 	isOn 		bool, on = true, off = false.
	 * 	@param 	intensity 	const pointer to const Vec4, intensity (colour) of the light.
	 * 	@param 	pos 		const pointer to const Vec3, position of the light.
	 */
	Light *(*new)(bool isOn, const Vec3 *const intensity, const Vec4 *const pos);

	/**
	 *	Free all memory associated with given light but not the Light object itself.
	 *
	 * 	@param 	light 	const pointer to const light, light to delete.
	 */
	void (*delete)(const Light *const light);

	/**
	 *	Get the position of the given light.
	 * 	
	 * 	@param 		light 	const pointer to const Light, light to get position of.
	 *
	 * 	@return  			Vec4, position of the light.
	 */
	Vec4 (*getPosition)(const Light *const light);

	/**
	 *	Set the position of the given light.
	 * 	
	 * 	@param 	light 		const pointer to const Light, light to set position of.
	 * 	@param 	position 	const pointer to const Vec4, position to move light to.
	 */
	void (*setPosition)(Light *const light, const Vec4 *const position);

	/**
	 *	Get the intensity (colour) of the given light.
	 *
	 * 	@param 	light 	const pointer to const Light, light to get intensity of.
	 *	
	 * 	@return 		Vec3, intensity of the light. 	
	 */
	Vec3 (*getIntensity)(const Light *const light);

	/**
	 *	Set the intensity of the given light.
	 *
	 *	@param 		light 		const pointer to const Light, light to set intensity of.
	 * 	@param 		intensity 	const pointer to const Vec3, intensity to set to.
	 */
	void (*setIntensity)(Light *const light, const Vec3 *const intensity);

	/**
	 *	Turn given light on.
	 *
	 * 	@param 	light 	const pointer to Light, light to turn on.
	 */
	void (*turnOn)(Light *const light);

	/**
	 * 	Turn given light off.
	 *
	 * 	@param 	light 	const pointer to Light, light to turn off.
	 */
	void (*turnOff)(Light *const light);
} LightManager;

extern const LightManager manLight;

#endif // COH_LIGHT
