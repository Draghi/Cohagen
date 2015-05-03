#ifndef COH_MOUSE_H
#define COH_MOUSE_H

typedef struct Window_s Window;

#include "Display.h"

extern const int MOUSE_BUTTON_1;
extern const int MOUSE_BUTTON_2;
extern const int MOUSE_BUTTON_3;
extern const int MOUSE_BUTTON_4;
extern const int MOUSE_BUTTON_5;
extern const int MOUSE_BUTTON_6;
extern const int MOUSE_BUTTON_7;
extern const int MOUSE_BUTTON_8;

extern const int MOUSE_BUTTON_LEFT;
extern const int MOUSE_BUTTON_RIGHT;
extern const int MOUSE_BUTTON_MIDDLE;

typedef struct Mouse_s {
	/** The old mouse x **/
	double omx;

	/** The old mouse y **/
	double omy;

	/** The old abs mouse x **/
	double oamx;

	/** The old abs mouse y **/
	double oamy;
} Mouse;

typedef struct MouseManager_s {
	/**Constructor to build a Mouse**/
	Mouse*(* new)();

	/**Deconstructor to ensure that the parent window is destroyed**/
	void(* delete)(Mouse* mouse);

	/**Returns the absolute x and y position of the mouse (relative to the monitor) [0] - x, [1] - y**/
	void(* getAbsolutePosition)(Window* window, double* pos);

	/**Polls mouse related "things"**/
	void(* update)(Window* window);

	/**Returns true if the given mouse button is down**/
	bool(* isDown)(Window* window, int button);

	/**Returns true if the given mouse button is up**/
	bool(* isUp)(Window* window, int button);

	/**Returns the x and y position of the mouse (relative to the window) [0] - x, [1] - y**/
	void(* getPosition)(Window* window, double* pos);

	/**Returns the x position of the mouse relative to the window**/
	double(* getX)(Window* window);

	/**Returns the y position of the mouse relative to the window**/
	double(* getY)(Window* window);

	/**Returns the absolute x position of the mouse relative to the monitor**/
	double(* getAbsoluteX)(Window* window);

	/**Returns the absolute y position of the mouse relative to the monitor**/
	double(* getAbsoluteY)(Window* window);

	/**Returns the x movement of the mouse since the last update, relative to the display.**/
	double(* getDX)(Window* window);

	/**Returns the y movement of the mouse since the last update, relative to the display.**/
	double(* getDY)(Window* window);

	/**Returns the x movement of the mouse since the last update, relative to the screen.**/
	double(* getAbsoluteDX)(Window* window);

	/**Returns the y movement of the mouse since the last update, relative to the screen.**/
	double(* getAbsoluteDY)(Window* window);

	/**Returns if the mouse/cursor is hovering inside the window.**/
	bool(* isInWindow)(Window* window);
} MouseManager;

extern const MouseManager manMouse;

#endif
