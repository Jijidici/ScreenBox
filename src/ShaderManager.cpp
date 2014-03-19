/* *****************************************************
 *	ShaderManager.cpp - Jijidici - 16/03/2014
 * ***************************************************** */
#include "ShaderManager.h"

#include <iostream>
#include <fstream>
#include <cassert>

ShaderManager::ShaderManager() {

}

ShaderManager::~ShaderManager() {
	for(std::map<std::string, GLuint>::iterator it=_shaderMap.begin(); it!=_shaderMap.end(); ++it) {
		glDeleteProgram(it->second);
		it->second = 0;
	}

	_shaderMap.clear();
	_locationMap.clear();
}

void ShaderManager::addShader(std::string sTag, const char* sVSPath, const char* sFSPath) {
	GLuint newProgramID = loadProgram(sVSPath, sFSPath);
	_shaderMap.insert(std::make_pair(sTag, newProgramID));
}

GLuint ShaderManager::getShader(std::string sTag) {
	std::map<std::string, GLuint>::iterator it = _shaderMap.find(sTag);
	assert(it != _shaderMap.end());
	return it->second;
}

void ShaderManager::addUniformLocation(std::string sShaderTag, std::string sUniformName, std::string sUniformTag) {
	GLuint shader = getShader(sShaderTag);
	GLuint location = glGetUniformLocation(shader, sUniformName.c_str());
	_locationMap.insert(std::make_pair(sUniformTag, location));
}

GLuint ShaderManager::getUniformLocation(std::string sUniformName) {
	std::map<std::string, GLuint>::iterator it = _locationMap.find(sUniformName);
	assert(it != _locationMap.end());
	return it->second;
}

GLuint ShaderManager::loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile) {
	const char* vertexShaderSource = readFile(vertexShaderFile);
    if(!vertexShaderSource) {
        std::cerr << "Unable to load " << vertexShaderFile << std::endl;
        return 0;
    }
    
    const char* fragmentShaderSource = readFile(fragmentShaderFile);
    if(!fragmentShaderSource) {
        std::cerr << "Unable to load " << fragmentShaderFile << std::endl;
        return 0;
    }
    
    // Creation d'un Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    // Sp�cification du code source
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    
    // Compilation du shader
    glCompileShader(vertexShader);
    
    /// V�rification que la compilation a bien fonctionn�e (tr�s important !)
    GLint compileStatus;
    
    // R�cup�ration du status de compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        // Si echec, r�cup�ration de la taille du log de compilation
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        
        // Allocation d'une chaine de caract�re suffisement grande pour contenir le log
        char* log = new char[logLength];
        
        glGetShaderInfoLog(vertexShader, logLength, 0, log);
        std::cerr << "Vertex Shader error:" << log << std::endl;
        std::cerr << vertexShaderSource << std::endl;
        
        delete [] log;
        return 0;
    }
    
    // Creation d'un Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Sp�cification du code source
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    
    // Compilation du shader
    glCompileShader(fragmentShader);
    
    /// V�rification que la compilation a bien fonctionn�e (tr�s important !)
    
    // R�cup�ration du status de compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        // Si echec, r�cup�ration de la taille du log de compilation
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        
        // Allocation d'une chaine de caract�re suffisement grande pour contenir le log
        char* log = new char[logLength];
        
        glGetShaderInfoLog(fragmentShader, logLength, 0, log);
        std::cerr << "Fragment Shader error:" << log << std::endl;
        std::cerr << fragmentShaderSource << std::endl;
        
        delete [] log;
        return 0;
    }

    GLuint program;
    
    // Creation d'un programme
    program = glCreateProgram();
    
    // Attachement des shaders au programme
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    // D�sallocation des shaders: ils ne seront r�ellement supprim�s que lorsque le programme sera supprim�
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Edition de lien
    glLinkProgram(program);
    
    /// V�rification que l'�dition de liens a bien fonctionn�e (tr�s important aussi !)
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE) {
        // Si echec, r�cup�ration de la taille du log de link
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        
        // Allocation d'une chaine de caract�re suffisement grande pour contenir le log
        char* log = new char[logLength];
        
        glGetProgramInfoLog(program, logLength, 0, log);
        std::cerr << "Program link error:" << log << std::endl;
        
        delete [] log;
        return 0;
    }
    
    delete [] vertexShaderSource;
    delete [] fragmentShaderSource;
    
    return program;
}

const char* ShaderManager::readFile(const char* sFilePath) {
    FILE * shaderFileDesc = NULL;
	fopen_s(&shaderFileDesc, sFilePath, "rb" );
    if (!shaderFileDesc) {
		std::cout << "[!] Unable to load file : " << sFilePath << std::endl;
		fclose(shaderFileDesc);
        return "";
	}

    fseek ( shaderFileDesc , 0 , SEEK_END );
    long fileSize = ftell ( shaderFileDesc );
    rewind ( shaderFileDesc );
    char * buffer = new char[fileSize + 1];
    fread( buffer, 1, fileSize, shaderFileDesc );
    buffer[fileSize] = '\0';
	fclose(shaderFileDesc);
	return buffer;
}