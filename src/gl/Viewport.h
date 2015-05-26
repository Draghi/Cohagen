#ifndef COH_VIEWPORT_H
#define COH_VIEWPORT_H

/**
 * Viewport Object
 */
typedef struct Viewport_s {

	float x;
	float y;
	float width;
	float height;

} Viewport;

typedef struct ViewportManager_s {

	/**
	 * Makes a call to the function glViewport
	 *
	 * @param viewport 			pointer to Viewport object being bound
	 */
	void (*makeActive)(const Viewport *const viewport);

	/**
	 * Getter for x
	 *
	 * @param viewport			pointer to viewport object
	 * @returns x
	 */
	float (*GetX)(const Viewport *const viewport);

	/**
	 * Setter for x
	 *
	 * @param viewport			pointer to viewport object
	 * @param newX				float value to set x
	 * @returns void
	 */
	void (*SetX)(const Viewport *const viewport, float newX);

	/**
	 * Getter for y
	 *
	 * @param viewport			pointer to viewport object
	 * @returns y
	 */
	float (*GetY)(const Viewport *const viewport);

	/**
	 * Setter for y
	 *
	 * @param viewport			pointer to viewport object
	 * @param newY				float value to set y
	 * @returns void
	 */
	void (*SetY)(const Viewport *const viewport, float newY);

	/**
	 * Getter for height
	 *
	 * @param viewport			pointer to viewport object
	 * @returns height
	 */
	float (*GetHeight)(const Viewport *const viewport);

	/**
	 * Setter for height
	 *
	 * @param viewport			pointer to viewport object
	 * @param newHeight			float value to set height
	 * @returns void
	 */
	void (*SetHeight)(const Viewport *const viewport, float newHeight);

	/**
	 * Getter for width
	 *
	 * @param viewport			pointer to viewport object
	 * @returns width
	 */
	float (*GetWidth)(const Viewport *const viewport);

	/**
	 * Setter for width
	 *
	 * @param viewport			pointer to viewport object
	 * @param newWidth			float value to set width
	 * @returns void
	 */
	void (*SetWidth)(const Viewport *const viewport, float newWidth);

} ViewportManager;

extern const ViewportManager manViewport;

#endif
