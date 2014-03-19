/* *****************************************************
 *	TextureManager.cpp - Jijidici - 16/03/2014
 * ***************************************************** */

#include "TextureManager.h"

#include <iostream>
#include <cassert>
#include "stb_image.h"

TextureManager::TextureManager() 
	: _namedTextures()
	, _processTextures(0) {
	
}

TextureManager::~TextureManager() {
	for(std::map<std::string, GLuint>::iterator it=_namedTextures.begin(); it!=_namedTextures.end(); ++it) {
		glDeleteTextures(1, &(it->second));
	}
	_namedTextures.end();

	if(_processTextures.size() > 0) {
		glDeleteTextures(_processTextures.size(), &(_processTextures[0]));
	}
}

void TextureManager::generateNamedTexture(std::string sTexName, const char* sImagePath, int iChannelCount) {
	int iTexX;
	int iTexY;
	int iTexComp;

	unsigned char* texData = stbi_load(sImagePath, &iTexX, &iTexY, &iTexComp, iChannelCount);
	std::cout << "-> Image " << sTexName << " loaded : " << iTexX << "x" << iTexY << ":" << iTexComp <<std::endl;

	// determine input format
	GLenum inputFormat;
	switch(iTexComp) {
	case 1:
		inputFormat = GL_RED;
		break;

	case 3:
		inputFormat = GL_RGB;
		break;

	case 4:
		inputFormat = GL_RGBA;
		break;

	default:
		std::cout << "[!] Unsupported image format for : " << sTexName <<std::endl;
		assert(false);
		break;
	}

	// build the texture
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, iChannelCount, iTexX, iTexY, 0, inputFormat, GL_UNSIGNED_BYTE, texData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	_namedTextures.insert(std::make_pair(sTexName, texId));
	delete texData;
}

void TextureManager::generateProcessTextures(int iTexCount) {
	if(_processTextures.size() > 0) {
		std::cout << "[!] Process textures already generated" << std::endl;
		assert(false);
	} else {
		_processTextures.resize(iTexCount);
		glGenTextures(iTexCount, &(_processTextures[0]));
		std::cout << "-> " << iTexCount << " were generated" << std::endl;
	}
}

GLuint TextureManager::getNamedTexture(std::string sTexName) {
	std::map<std::string, GLuint>::iterator it = _namedTextures.find(sTexName);
	assert(it != _namedTextures.end());
	return it->second;
}

GLuint TextureManager::getProcessTexture(unsigned int iTexId) {
	GLuint ret = 0;
	if(iTexId < _processTextures.size()) {
		ret = _processTextures[iTexId];
	}
	return ret;
}

void TextureManager::initProcessTexture(unsigned int iTexId, int iW, int iH, int internalFormat, GLenum inputFormat, GLenum dataType) {
	GLuint texId = getProcessTexture(iTexId);
	glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, iW, iH, 0, inputFormat, dataType, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void TextureManager::bindTexture(std::string sTexName, GLenum textureUnit) {
	GLuint texId = getNamedTexture(sTexName);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void TextureManager::bindTexture(unsigned int iTexId, GLenum textureUnit) {
	GLuint texId = getProcessTexture(iTexId);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void TextureManager::unbindTexture(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}