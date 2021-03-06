/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#ifndef __SCREEN_BOX_H__
#define __SCREEN_BOX_H__

#include "GL/glew.h"
#include "glfw3.h"

class ScreenBox {
public:
	void init();
	void launch();
	void destroy();


private:
	static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

	GLFWwindow* _pWindow;
	int _iW;
	int _iH;
	double _dTime;
	float _fFPS;
};

#endif //__SCREEN_BOX_H__