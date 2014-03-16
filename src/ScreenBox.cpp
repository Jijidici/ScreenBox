/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#include "ScreenBox.h"

#define GLFW_DLL

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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
	// QUAD
	_iQuadTriangleCount = 2;
	int quad_triangleList[] = {0, 1, 2, 2, 1, 3};
	float quad_vertices[] = { -0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f };
	float quad_normals[] = { 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f };
	float quad_uv[] = {0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f };

	// SPACE MODEL
	std::vector<int> space_triangleList;
	std::vector<float> space_vertices;
	std::vector<float> space_normals;
	std::vector<float> space_uv;

	// load model from file
	_iSpaceTriangleCount = 0;
	int iVerticesCount = 0;
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile("models/voyager/voyager_carbajal.3ds", aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	for(unsigned int iMesh=0; iMesh<pScene->mNumMeshes; ++iMesh) {
		const aiMesh* pMesh = pScene->mMeshes[iMesh];
		int iPrevVerticesCount = space_vertices.size();

		for(unsigned int iVert=0; iVert<pMesh->mNumVertices; ++iVert) {
			aiVector3D pPos = pMesh->mVertices[iVert];
			aiVector3D pNormal = pMesh->mNormals[iVert];
			aiVector3D pTexCoord(0.f, 0.f, 0.f);
			if(pMesh->HasTextureCoords(0)) {
				pTexCoord = pMesh->mTextureCoords[0][iVert];
			}

			space_vertices.push_back(pPos.x); space_vertices.push_back(pPos.y); space_vertices.push_back(pPos.z); 
			space_normals.push_back(pNormal.x); space_normals.push_back(pNormal.y); space_normals.push_back(pNormal.z); 
			space_uv.push_back(pTexCoord.x); space_uv.push_back(pTexCoord.y);
			++iVerticesCount;
		}

		for(unsigned int iFace=0; iFace<pMesh->mNumFaces; ++iFace) {
			const aiFace& Face = pMesh->mFaces[iFace];
			assert(Face.mNumIndices == 3);
			space_triangleList.push_back(iPrevVerticesCount + Face.mIndices[0]);
			space_triangleList.push_back(iPrevVerticesCount + Face.mIndices[1]);
			space_triangleList.push_back(iPrevVerticesCount + Face.mIndices[2]);
			++_iSpaceTriangleCount;
		}
	}
	std::cout << "-> Space model loaded : " << _iSpaceTriangleCount << " faces, " << iVerticesCount << " vertices" << std::endl;

	// Build vaos and vbos
	// SPACE MODEL
	glGenVertexArrays(1, &_spaceVAO);
	glGenBuffers(4, _spaceVBOs);
	
	glBindVertexArray(_spaceVAO);
	// Indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _spaceVBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, space_triangleList.size()*sizeof(int), &(space_triangleList[0]), GL_STATIC_DRAW);
    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, _spaceVBOs[1]);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, space_vertices.size()*sizeof(float), &(space_vertices[0]), GL_STATIC_DRAW);
    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, _spaceVBOs[2]);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, space_normals.size()*sizeof(float), &(space_normals[0]), GL_STATIC_DRAW);
    // UVs
    glBindBuffer(GL_ARRAY_BUFFER, _spaceVBOs[3]);
	glEnableVertexAttribArray(TEXCOORD_LOCATION);
    glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, space_uv.size()*sizeof(float), &(space_uv[0]), GL_STATIC_DRAW);

	// QUAD
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
	_pSM = new ShaderManager();
	_pSM->addShader("basic", "shaders/basic.vs", "shaders/basic.fs");
	_pSM->addUniformLocation("basic", "uMatProjection");
	_pSM->addUniformLocation("basic", "uMatView");
	_pSM->addUniformLocation("basic", "uMatModel");
	_pSM->addUniformLocation("basic", "uTexture1");

	// Build texture
	_pTM = new TextureManager();
	_pTM->generateNamedTexture("texTest", "models/voyager/texures/voyagerwashboard.png", 4);
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

		// Compute space matrices
		glm::mat4 worldToScreen = glm::perspective(60.f, static_cast<float>(_iW)/static_cast<float>(_iH), 0.1f, 100.f);
		glm::mat4 worldToView = glm::lookAt(glm::vec3(0.f, 0.f, -10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 objectToWorld = glm::mat4(1.f);

		//draw basic quad
		glUseProgram(_pSM->getShader("basic"));
		glUniformMatrix4fv(_pSM->getUniformLocation("uMatProjection"), 1, GL_FALSE, glm::value_ptr(worldToScreen));
		glUniformMatrix4fv(_pSM->getUniformLocation("uMatView"), 1, GL_FALSE, glm::value_ptr(worldToView));
		glUniformMatrix4fv(_pSM->getUniformLocation("uMatModel"), 1, GL_FALSE, glm::value_ptr(objectToWorld));
		glUniform1i(_pSM->getUniformLocation("uTexture1"), 0);

		_pTM->bindTexture("texTest", GL_TEXTURE0);

		glBindVertexArray(_spaceVAO);
		glDrawElementsInstanced(GL_TRIANGLES, _iSpaceTriangleCount*3, GL_UNSIGNED_INT, (void*)0, 1);

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
	glDeleteVertexArrays(1, &_spaceVAO);
	glDeleteBuffers(4, _quadVBOs);
	glDeleteBuffers(4, _spaceVBOs);

	delete _pSM;

	glfwDestroyWindow(_pWindow);
	glfwTerminate();
}

void ScreenBox::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//exit the app
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}