/* *****************************************************
 *	Light.h - Jijidici - 19/03/2014
 * ***************************************************** */
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "glm/glm.hpp"

class Light {
public:
	Light(glm::vec3 position, glm::vec3 target, glm::vec3 color, float fIntensity, float fLength);

	inline glm::vec3 getPosition() { return _position; }
	inline glm::vec3 getTarget() { return _target; }
	inline glm::vec3 getColor() { return _color; }
	inline float getIntensity() { return _fIntensity; }
	inline float getLength() { return _fLength; }

private:
	glm::vec3 _position;
	glm::vec3 _target;
	glm::vec3 _color;
	float _fIntensity;
	float _fLength;
};

#endif //__LIGHT_H__