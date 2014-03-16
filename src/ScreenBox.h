/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#ifndef __SCREEN_BOX_H__
#define __SCREEN_BOX_H__

#include <map>
#include <vector>
#include "GL/glew.h"
#include "glfw3.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "TrackBallCamera.h"

class ScreenBox {
public:
	void init();
	void launch();
	void destroy();

private:
	static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void onScroll(GLFWwindow* window, double xoffset, double yoffset);
	static void onCursorPos(GLFWwindow* window, double xpos, double ypos);
	static void onMouseButton(GLFWwindow* window, int button, int action, int mods);

	GLFWwindow* _pWindow;
	int _iW;
	int _iH;
	double _dTime;
	float _fFPS;

	int _iQuadTriangleCount;
	int _iSpaceTriangleCount;
	GLuint _quadVBOs[4];
	GLuint _quadVAO;
	std::map<GLuint, std::vector<GLuint>> _spaceVertexBuffers;
	
	ShaderManager* _pSM;
	TextureManager* _pTM;
};

#endif //__SCREEN_BOX_H__