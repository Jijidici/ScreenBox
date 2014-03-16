/* *****************************************************
 *	ShaderManager.h - Jijidici - 16/03/2014
 * ***************************************************** */
#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include <map>
#include <string>
#include "GL/glew.h"

class ShaderManager {
public:
	~ShaderManager();
	static ShaderManager* getInstance();
	GLuint addShader(std::string sTag, const char* sVSPath, const char* sFSPath);
	GLuint getShader(std::string sTag);

private:
	ShaderManager();
	GLuint loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
	const char* readFile(const char* sFilePath);

	static ShaderManager* _pInstance;
	std::map<std::string, GLuint> _shaderMap;
};

#endif //__SHADER_MANAGER_H__