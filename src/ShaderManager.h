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
	ShaderManager();
	~ShaderManager();

	void addShader(std::string sTag, const char* sVSPath, const char* sFSPath);
	GLuint getShader(std::string sTag);

	void addUniformLocation(std::string sShaderTag, std::string sUniformName);
	GLuint getUniformLocation(std::string sUniformName);


private:
	GLuint loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
	const char* readFile(const char* sFilePath);

	std::map<std::string, GLuint> _shaderMap;
	std::map<std::string, GLuint> _locationMap;
};

#endif //__SHADER_MANAGER_H__