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


} ViewportManager;

#endif
