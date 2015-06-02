#ifndef COH_CAMERA_H
#define COH_CAMERA_H

#include "math/Vec3.h"
#include "render/RenderObject.h"
#include "gl/Viewport.h"
#include "MatrixManager.h"

typedef struct Camera_s Camera;


typedef struct Camera_s {

	/// position of the camera struct
	Vec3 position;

	/// rotation of the camera struct
	Vec3 rotation;

	/// scale of the camera struct
	Vec3 scale;

	/// field of view of the camera struct
	scalar fov;

	/// nearest value of z to be rendered from
	scalar zNear;

	/// farthest value of z to be rendered to
	scalar zFar;

	///Viewport object pointer
	Viewport* viewportObject;

	///void pointer to a parent game object
	RenderObject* parentObject;

} Camera;

typedef struct CameraManager_s {

	/**
	 * Creates a new camera "object"
	 *
	 * @param position		position of the camera object
	 * @param rotation		rotation of the camera object
	 * @param scale			scale of the camera object
	 * @returns camera		a pointer to a camera object
	 */
	Camera *(*new)(Vec3* position, Vec3* rotation, Vec3* scale);

	/**
	 * binds the camera object
	 */
	void (*bind)(Camera* camera, MatrixManager* matrixMan);

	/**
	 * unbinds the camera object
	 */
	void (*unbind)();

	/**
	 * sets the position of the camera object
	 *
	 * @param camera 		a pointer to a Camera object
	 * @param x				value for x-axis
	 * @param y				value for y-axis
	 * @param z				value for z-axis
	 * @returns void
	 */
	void (*setPositionXYZ)(Camera* camera, scalar x, scalar y, scalar z);

	/**
	 * adds to the position of the camera object
	 *
	 * @param camera 		a pointer to a Camera object
	 * @param x				value for x-axis
	 * @param y				value for y-axis
	 * @param z				value for z-axis
	 * @returns void
	 */
	void (*addPositionXYZ)(Camera* camera, scalar x, scalar y, scalar z);

	/**
	 * sets the position of the camera object
	 *
	 * @param camera		pointer to a Camera object
	 * @param newPosition	the new position for the camera to be set
	 * @returns void
	 */
	void (*setPositionVec)(Camera* camera, Vec3* newPosition);

	/**
	 * adds to the position of the camera object
	 *
	 * @param camera		a pointer to a Camera object
	 * @param displacement	the displacement of the camera from its previous position
	 * @returns void
	 */
	void (*addPositionVec)(Camera* camera, Vec3* displacement);

	/**
	 * sets the rotation of the camera object
	 *
	 * @param camera		a pointer to a Camera object
	 * @param x				value for x-axis rotation
	 * @param y				value for y-axis rotation
	 * @param z				value for z-axis rotation
	 * @returns void
	 */
	void (*setRotationXYZ)(Camera* camera, scalar x, scalar y, scalar z);

	/**
	 * adds to the rotation of the camera object
	 *
	 * @param camera		pointer to a Camera object
	 * @param x				value for x-axis rotation
	 * @param y 			value for y-axis rotation
	 * @param z				value for z-axis rotation
	 * @returns void
	 */
	void (*addRotationXYZ)(Camera* camera, scalar x, scalar y, scalar z);

	/**
	 * sets the rotation of the camera object
	 *
	 * @param camera 		A pointer to a Camera object
	 * @param rotation		The rotation of the camera object to be set
	 * @returns void
	 */
	void (*setRotationVec)(Camera* camera, Vec3* rotation);

	/**
	 * adds to the rotation of the camera object
	 *
	 * @param camera 		A pointer to a Camera object
	 * @param rotation		the additional rotation to be applied to the Camera object
	 * @returns void
	 */
	void (*addRotationVec)(Camera* camera, Vec3* rotation);

	/**
	 * sets the scale of the camera object
	 *
	 * @param camera 		A pointer to a Camera object
	 * @param x				value for x-axis
	 * @param y				value for y-axis
	 * @param z				value for z-axis
	 * @returns void
	 */
	void (*setScaleXYZ)(Camera* camera, scalar x, scalar y, scalar z);

	/**
	 * adds to the scale of the camera object
	 *
	 * @param camera 		A pointer to a Camera object
	 * @param x				value for x-axis
	 * @param y 			value for y-axis
	 * @param z				value for z-axis
	 * @returns void
	 */
	void (*addScaleXYZ)(Camera* camera, scalar x, scalar y, scalar z);

	/**
	 * sets the scale of the camera object
	 *
	 * @param camera		A pointer to a Camera object
	 * @param scale 		The scale of the Camera object to be set
	 * @returns void
	 */
	void (*setScaleVec)(Camera* camera, Vec3* scale);

	/**
	 * adds to the scale of the camera object
	 *
	 * @param camera 		A pointer to a Camera object
	 * @param scale 		The scale to be added to the camera object
	 * @returns void
	 */
	void (*addScaleVec)(Camera* camera, Vec3* scale);

	/**
	 * Sets the viewport object for the camera object
	 *
	 * @param camera 		A pointer to a Camera object
	 * @param viewport		A pointer to a viewport object
	 * @returns void
	 */
	void (*setViewportObject)(Camera* camera, Viewport* viewportObject);

	/**
	 * Sets the render object for the camera object
	 *
	 * @param camera 		A pointer to a Camera object
	 * @param renderObject	A pointer to a render object
	 * @returns void
	 */
	void (*setParentRenderObject)(Camera* camera, RenderObject* renderObject);

	/**
	 * Sets the projection info for the Camera object
	 *
	 * @param camera		A pointer to a Camera object
	 * @param fov			field-of-view for the Camera object projection
	 * @param zNear			nearest z value to render from
	 * @param zFar 			farthest z value to render to
	 * @returns void
	 */
	void (*setProjectionInfo)(Camera* camera, scalar fov, scalar zNear, scalar zFar);

	/**
	 * Frees the memory allocated by the Camera pointer
	 *
	 * @params camera		A pointer to a Camera object
	 * @returns void
	 */
	void (*delete)(Camera *const camera);

} CameraManager;

extern const CameraManager manCamera;

#endif
