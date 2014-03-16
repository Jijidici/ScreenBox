/* *****************************************************
 *	TextureManager.h - Jijidici - 16/03/2014
 * ***************************************************** */
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <map>
#include <vector>
#include <string>
#include "GL/glew.h"

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	void generateNamedTexture(std::string sTexName, const char* sImagePath, int iChannelCount);
	void generateProcessTextures(int iTexCount);
	GLuint getNamedTexture(std::string sTexName);
	GLuint getProcessTexture(unsigned int iTexId);
	void initProcessTexture(unsigned int iTexId, int iW, int iH, int internalFormat, GLenum inputFormat, GLenum dataType);
	void bindTexture(std::string sTexName, GLenum textureUnit);
	void bindTexture(unsigned int iTexId, GLenum textureUnit);

private:
	std::map<std::string, GLuint> _namedTextures;
	std::vector<GLuint> _processTextures;
};

#endif //__TEXTURE_MANAGER_H__