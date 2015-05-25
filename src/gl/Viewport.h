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
	 * @param viewport pointer to Viewport object being bound
	 */
	void (*makeActive)(const Viewport *const);

	/**
	 * Getter for x
	 *
	 * @returns x
	 */
	float (*GetX)();

	/**
	 * Setter for x
	 *
	 * @param newX		float value to set x
	 * @returns void
	 */
	void (*SetX)(float newX);

	/**
	 * Getter for y
	 *
	 * @returns y
	 */
	float (*GetY)();

	/**
	 * Setter for y
	 *
	 * @param newY		float value to set y
	 * @returns void
	 */
	void (*SetY)(float newY);

	/**
	 * Getter for height
	 *
	 * @returns height
	 */
	float (*GetHeight)();

	/**
	 * Setter for height
	 *
	 * @param newHeight		float value to set height
	 * @returns void
	 */
	void (*SetHeight)(float newHeight);

	/**
	 * Getter for width
	 *
	 * @returns width
	 */
	float (*GetWidth)();

	/**
	 * Setter for width
	 *
	 * @param newWidth		float value to set width
	 * @returns void
	 */
	void (*SetWidth)(float newWidth);

} ViewportManager;

#endif
