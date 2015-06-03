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

/**
 *	Class used to manage Viewport objects.
 */
typedef struct ViewportManager_s {

	/**
	 * Creates a new viewport "object".
	 * @param x	the location along the x-axis where the viewport originates
	 * @param y the location along the y-axis where the viewport originates
	 * @param width the width of the viewport
	 * @param height the height of the viewport
	 * @return a new Camera "object".
	 */
	Viewport *(*new)(float x, float y, float width, float height);

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
	float (*getX)(const Viewport *const viewport);

	/**
	 * Setter for x
	 *
	 * @param viewport			pointer to viewport object
	 * @param newX				float value to set x
	 * @returns void
	 */
	void (*setX)(Viewport *const viewport, float newX);

	/**
	 * Getter for y
	 *
	 * @param viewport			pointer to viewport object
	 * @returns y
	 */
	float (*getY)(const Viewport *const viewport);

	/**
	 * Setter for y
	 *
	 * @param viewport			pointer to viewport object
	 * @param newY				float value to set y
	 * @returns void
	 */
	void (*setY)(Viewport *const viewport, float newY);

	/**
	 * Getter for height
	 *
	 * @param viewport			pointer to viewport object
	 * @returns height
	 */
	float (*getHeight)(const Viewport *const viewport);

	/**
	 * Setter for height
	 *
	 * @param viewport			pointer to viewport object
	 * @param newHeight			float value to set height
	 * @returns void
	 */
	void (*setHeight)(Viewport *const viewport, float newHeight);

	/**
	 * Getter for width
	 *
	 * @param viewport			pointer to viewport object
	 * @returns width
	 */
	float (*getWidth)(const Viewport *const viewport);

	/**
	 * Setter for width
	 *
	 * @param viewport			pointer to viewport object
	 * @param newWidth			float value to set width
	 * @returns void
	 */
	void (*setWidth)(Viewport *const viewport, float newWidth);

	/**
	 * Frees the memory assigned to the viewport object pointer
	 *
	 * @param viewport			pointer to viewport object
	 * @returns void
	 */
	void (*delete)(Viewport *const viewport);

} ViewportManager;

extern const ViewportManager manViewport;

#endif
