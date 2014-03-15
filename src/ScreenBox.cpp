/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#include "ScreenBox.h"

#define GLFW_DLL

#include <iostream>
#include "GL/glew.h"
#include "glfw3.h"

void ScreenBox::init() {
	std::cout << "> INIT SCREENBOX" <<std::endl;

	_iW = 800;
	_iH = 600;
	_bLoop = true;

	 // Initialise GLFW
    if(!glfwInit()) {
        std::cout << "Failed to initialize GLFW" <<std::endl;
        exit(EXIT_FAILURE);
    }

	// Open a window
	GLFWwindow* window = glfwCreateWindow(_iW, _iH, "ScreenBox", NULL, NULL);
}

void ScreenBox::launch() {
	std::cout << "> LAUNCH SCREENBOX" <<std::endl;

	while(_bLoop) {

	}
}

void ScreenBox::destroy() {
	std::cout << "> DESTROY SCREENBOX" <<std::endl;
}