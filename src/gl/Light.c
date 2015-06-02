#include "Light.h"

static Light *newDefault() {
	Light *light = malloc(sizeof(Light));

	light->isOn = false;
	light->intensity = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);
	light->pos = manVec4.create(NULL, 0.0f, 0.0f, 0.0f, 0.0f);

	return light;
}

static Light *new(bool isOn, const Vec3 *const intensity, const Vec4 *const pos) {
	Light *light = malloc(sizeof(Light));

	light->isOn = isOn;
	light->intensity = *intensity;
	light->pos = *pos;

	return light;
}

static void delete(const Light *const light) {

}

static Vec4 getPosition(const Light *const light) {
	return light->pos;
}

static void setPosition(Light *const light, const Vec4 *const position) {
	light->pos = *position;
}

static Vec3 getIntensity(const Light *const light) {
	return light->intensity;
}

static void setIntensity(Light *const light, const Vec3 *const intensity) {
	light->intensity = *intensity;
}

static void turnOn(Light *const light) {
	light->isOn = true;
}

static void turnOff(Light *const light) {
	light->isOn = false;
}

const LightManager manLight = {newDefault, new, delete, getPosition, setPosition, getIntensity, setIntensity, turnOn, turnOff};
