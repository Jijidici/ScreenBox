/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#include "ScreenBox.h"

#define GLFW_DLL

#include <iostream>
#include <string>
#include <sstream>
#include "ShaderManager.h"

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEXCOORD_LOCATION 2

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

	// Iniitalise glew
	GLenum err = glewInit();
    if (GLEW_OK != err) {
          std::cout << "Error: " << glewGetErrorString(err) <<std::endl;
          exit( EXIT_FAILURE );
    }

	// OpenGL configs
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);

	// Init geometry
	_iQuadTriangleCount = 2;
	int quad_triangleList[] = {0, 1, 2, 2, 1, 3};
	float quad_vertices[] = { -0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f };
	float quad_normals[] = { 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f };
	float quad_uv[] = {0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f };

	// Build vaos and vbos
	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(4,_quadVBOs);

	glBindVertexArray(_quadVAO);
	// Indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadVBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_triangleList), quad_triangleList, GL_STATIC_DRAW);
    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, _quadVBOs[1]);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, _quadVBOs[2]);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_normals), quad_normals, GL_STATIC_DRAW);
    // UVs
    glBindBuffer(GL_ARRAY_BUFFER, _quadVBOs[3]);
	glEnableVertexAttribArray(TEXCOORD_LOCATION);
    glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_uv), quad_uv, GL_STATIC_DRAW);

	// Build shaders
	GLuint basicShader = ShaderManager::getInstance()->addShader("basic", "shaders/basic.vs", "shaders/basic.fs");
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

		GLuint basicShader = ShaderManager::getInstance()->getShader("basic");
		glUseProgram(basicShader);

		glBindVertexArray(_quadVAO);
		glDrawElementsInstanced(GL_TRIANGLES, _iQuadTriangleCount*3, GL_UNSIGNED_INT, (void*)0, 1);

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

	glDeleteVertexArrays(1, &_quadVAO);
	glDeleteBuffers(4, _quadVBOs);

	glfwDestroyWindow(_pWindow);
	glfwTerminate();
}

void ScreenBox::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//exit the app
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}