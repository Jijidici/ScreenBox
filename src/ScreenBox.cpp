/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#include "ScreenBox.h"

#define GLFW_DLL

#include <iostream>
#include <string>
#include <sstream>

void ScreenBox::init() {
	std::cout << "> INIT SCREENBOX" <<std::endl;

	 _pWindow = NULL;
	_iW = 800;
	_iH = 600;

	 // Initialise GLFW
    if(!glfwInit()) {
        std::cout << "Failed to initialize GLFW" <<std::endl;
        exit(EXIT_FAILURE);
    }

	// Open a window
	_pWindow = glfwCreateWindow(_iW, _iH, "ScreenBox", NULL, NULL);
	if (!_pWindow) {
        std::cout << "Failed to create a window" <<std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	// Handle key events
	glfwSetKeyCallback(_pWindow, ScreenBox::onKey);

	// Use OpenGL context
	glfwMakeContextCurrent(_pWindow);

	// OpenGL configs
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
}

void ScreenBox::launch() {
	std::cout << "> LAUNCH SCREENBOX" <<std::endl;

	while(!glfwWindowShouldClose(_pWindow)) {
		_dTime = glfwGetTime();

		/* *************************************************** *
		 * ********* RENDER ZONE	
		 * *************************************************** */
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, _iW, _iH);

		glfwSwapBuffers(_pWindow);

		/* *************************************************** *
		 * ********* EVENT ZONE	
		 * *************************************************** */
		glfwPollEvents();

		//manage fps
		double dNewTime = glfwGetTime();
		_fFPS = 1.f/static_cast<float>(dNewTime-_dTime);

		std::stringstream fpsStream;
		fpsStream << "Screen Box - " << _fFPS << "FPS";
		glfwSetWindowTitle(_pWindow, fpsStream.str().c_str());
	}
}

void ScreenBox::destroy() {
	std::cout << "> DESTROY SCREENBOX" <<std::endl;
	glfwDestroyWindow(_pWindow);
	glfwTerminate();
}

void ScreenBox::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//exit the app
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}