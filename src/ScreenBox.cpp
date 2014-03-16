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
#include "MouseHandling.h"

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEXCOORD_LOCATION 2

void ScreenBox::init() {
	std::cout << "> INIT SCREENBOX" <<std::endl;

	_pWindow = NULL;
	_iW = 1200;
	_iH = 700;

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

	// Handle events
	glfwSetKeyCallback(_pWindow, ScreenBox::onKey);
	glfwSetScrollCallback(_pWindow, ScreenBox::onScroll);
	glfwSetCursorPosCallback(_pWindow, ScreenBox::onCursorPos);
	glfwSetMouseButtonCallback(_pWindow, ScreenBox::onMouseButton);

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

	// Builds geometry
	// QUAD
	_iQuadTriangleCount = 2;
	int quad_triangleList[] = {0, 1, 2, 2, 1, 3};
	float quad_vertices[] = { -0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f };
	float quad_normals[] = { 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f };
	float quad_uv[] = {0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f };

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

	// SPACE MODEL
	// load model from file
	_iSpaceTriangleCount = 0;
	int iVerticesCount = 0;
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile("models/kerrigan/Kerrigan_infested.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
	for(unsigned int iMesh=0; iMesh<pScene->mNumMeshes; ++iMesh) {
		const aiMesh* pMesh = pScene->mMeshes[iMesh];
		std::vector<int> space_triangleList;
		std::vector<float> space_vertices;
		std::vector<float> space_normals;
		std::vector<float> space_uv;

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
			space_triangleList.push_back(Face.mIndices[0]);
			space_triangleList.push_back(Face.mIndices[1]);
			space_triangleList.push_back(Face.mIndices[2]);
			++_iSpaceTriangleCount;
		}

		GLuint meshVAO;
		std::vector<GLuint> meshVBOS(4);
		glGenVertexArrays(1, &meshVAO);
		glGenBuffers(4, &(meshVBOS[0]));
	
		glBindVertexArray(meshVAO);
		// Indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVBOS[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, space_triangleList.size()*sizeof(int), &(space_triangleList[0]), GL_STATIC_DRAW);
		// Vertices
		glBindBuffer(GL_ARRAY_BUFFER, meshVBOS[1]);
		glEnableVertexAttribArray(POSITION_LOCATION);
		glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, space_vertices.size()*sizeof(float), &(space_vertices[0]), GL_STATIC_DRAW);
		// Normals
		glBindBuffer(GL_ARRAY_BUFFER, meshVBOS[2]);
		glEnableVertexAttribArray(NORMAL_LOCATION);
		glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, space_normals.size()*sizeof(float), &(space_normals[0]), GL_STATIC_DRAW);
		// UVs
		glBindBuffer(GL_ARRAY_BUFFER, meshVBOS[3]);
		glEnableVertexAttribArray(TEXCOORD_LOCATION);
		glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, space_uv.size()*sizeof(float), &(space_uv[0]), GL_STATIC_DRAW);

		_spaceVertexBuffers.insert(std::make_pair(meshVAO, meshVBOS));
	}
	std::cout << "-> Space model loaded : " << _iSpaceTriangleCount << " faces, " << iVerticesCount << " vertices" << std::endl;

	// Build shaders
	_pSM = new ShaderManager();
	_pSM->addShader("basic", "shaders/basic.vs", "shaders/basic.fs");
	_pSM->addUniformLocation("basic", "uMatProjection");
	_pSM->addUniformLocation("basic", "uMatView");
	_pSM->addUniformLocation("basic", "uMatModel");
	_pSM->addUniformLocation("basic", "uCameraPosition");
	_pSM->addUniformLocation("basic", "uDiffuse");
	_pSM->addUniformLocation("basic", "uSpec");

	// Build texture
	_pTM = new TextureManager();
	_pTM->generateNamedTexture("eye_diff", "models/kerrigan/Kerrigan_inf_eye_D.tga", 3);
	_pTM->generateNamedTexture("eye_spec", "models/kerrigan/Kerrigan_inf_eye_S.tga", 3);
	_pTM->generateNamedTexture("head_diff", "models/kerrigan/Kerrigan_inf_head_D.tga", 3);
	_pTM->generateNamedTexture("head_spec", "models/kerrigan/Kerrigan_inf_head_S.tga", 3);
	_pTM->generateNamedTexture("legswings_diff", "models/kerrigan/Kerrigan_inf_legswings_D.tga", 3);
	_pTM->generateNamedTexture("legswings_spec", "models/kerrigan/Kerrigan_inf_legswings_S.tga", 3);
	_pTM->generateNamedTexture("torso_diff", "models/kerrigan/Kerrigan_inf_torso_D.tga", 3);
	_pTM->generateNamedTexture("torso_spec", "models/kerrigan/Kerrigan_inf_torso_S.tga", 3);

	// Camera manipulation data
	MouseHandling::getInstance()->bLeftMousePressed = false;
	MouseHandling::getInstance()->savedPosX = 0.;
	MouseHandling::getInstance()->savedPosY = 0.;
}

void ScreenBox::launch() {
	std::cout << "> LAUNCH SCREENBOX" <<std::endl;

	while(!glfwWindowShouldClose(_pWindow)) {
		_dTime = glfwGetTime();

		/* *************************************************** *
		 * ********* RENDER ZONE	
		 * *************************************************** */
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, _iW, _iH);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute space matrices
		glm::mat4 worldToScreen = glm::perspective(60.f, static_cast<float>(_iW)/static_cast<float>(_iH), 0.1f, 100.f);
		glm::mat4 worldToView = TrackBallCamera::getInstance()->getViewMatrix();  // JOJO ! C'est ici que je récupère la matrice de vue de la caméra, il suffit que le programme de détection la mette ici
		glm::mat4 objectToWorld = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -2.f, 0.f));
		objectToWorld = glm::rotate(objectToWorld, 180.f, glm::vec3(0.f, 1.f, 0.f));

		//draw basic quad
		glUseProgram(_pSM->getShader("basic"));
		glUniformMatrix4fv(_pSM->getUniformLocation("uMatProjection"), 1, GL_FALSE, glm::value_ptr(worldToScreen));
		glUniformMatrix4fv(_pSM->getUniformLocation("uMatView"), 1, GL_FALSE, glm::value_ptr(worldToView));
		glUniformMatrix4fv(_pSM->getUniformLocation("uMatModel"), 1, GL_FALSE, glm::value_ptr(objectToWorld));
		glUniform3fv(_pSM->getUniformLocation("uCameraPosition"), 1, glm::value_ptr(TrackBallCamera::getInstance()->getCameraPosition()));
		glUniform1i(_pSM->getUniformLocation("uDiffuse"), 0);
		glUniform1i(_pSM->getUniformLocation("uSpec"), 1);

		std::vector<std::string> kerriganTexNames;
		kerriganTexNames.push_back("eye_diff");
		kerriganTexNames.push_back("eye_spec");
		kerriganTexNames.push_back("legswings_diff");
		kerriganTexNames.push_back("legswings_spec");
		kerriganTexNames.push_back("head_diff");
		kerriganTexNames.push_back("head_spec");
		kerriganTexNames.push_back("torso_diff");
		kerriganTexNames.push_back("torso_spec");

		int iNameCursor = 0;
		for(std::map<GLuint, std::vector<GLuint>>::iterator it=_spaceVertexBuffers.begin(); it!=_spaceVertexBuffers.end(); ++it) {
			_pTM->bindTexture(kerriganTexNames[iNameCursor++], GL_TEXTURE0);
			_pTM->bindTexture(kerriganTexNames[iNameCursor++], GL_TEXTURE1);

			glBindVertexArray(it->first);
			glDrawElementsInstanced(GL_TRIANGLES, _iSpaceTriangleCount*3, GL_UNSIGNED_INT, (void*)0, 1);
		}

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
	for(std::map<GLuint, std::vector<GLuint>>::iterator it=_spaceVertexBuffers.begin(); it!=_spaceVertexBuffers.end(); ++it) {
		glDeleteVertexArrays(1, &(it->first));
		glDeleteBuffers(4, &(it->second[0]));
	}

	delete _pSM;
	delete _pTM;

	glfwDestroyWindow(_pWindow);
	glfwTerminate();
}

void ScreenBox::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//exit the app
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void ScreenBox::onScroll(GLFWwindow* window, double xoffset, double yoffset) {
	if(yoffset < 0) {
		TrackBallCamera::getInstance()->moveFront(0.5f);
	} else {
		TrackBallCamera::getInstance()->moveFront(-0.5f);
	}
}

void ScreenBox::onCursorPos(GLFWwindow* window, double xpos, double ypos) {
	MouseHandling* pMouse = MouseHandling::getInstance();
	if(pMouse->bLeftMousePressed) {
		double horiMove = 0.5*(xpos-pMouse->savedPosX);
		double vertMove = 0.5*(pMouse->savedPosY-ypos);

		TrackBallCamera::getInstance()->rotateLeft(static_cast<float>(horiMove));
		TrackBallCamera::getInstance()->rotateUp(static_cast<float>(vertMove));
	}
	pMouse->savedPosX = xpos;
	pMouse->savedPosY = ypos;
}

void ScreenBox::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if(button == GLFW_MOUSE_BUTTON_1) {
		MouseHandling* pMouse = MouseHandling::getInstance();
			
		if(action == GLFW_PRESS) {
			pMouse->bLeftMousePressed = true;
		} else if(action == GLFW_RELEASE) {
			pMouse->bLeftMousePressed = false;
		}

	}
}