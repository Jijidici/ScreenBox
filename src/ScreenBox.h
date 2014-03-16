/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#ifndef __SCREEN_BOX_H__
#define __SCREEN_BOX_H__

#include "GL/glew.h"
#include "glfw3.h"
#include "ShaderManager.h"
#include "TextureManager.h"

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

	int _iQuadTriangleCount;
	int _iSpaceTriangleCount;
	GLuint _quadVBOs[4];
	GLuint _spaceVBOs[4];
	GLuint _quadVAO;
	GLuint _spaceVAO;
	

	ShaderManager* _pSM;
	TextureManager* _pTM;
};

#endif //__SCREEN_BOX_H__