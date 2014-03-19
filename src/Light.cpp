/* *****************************************************
 *	Light.cpp - Jijidici - 19/03/2014
 * ***************************************************** */
#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 target, glm::vec3 color, float fIntensity, float fLength)
	: _position(position)
	, _target(target)
	, _color(color)
	, _fIntensity(fIntensity)
	, _fLength(fLength) {

}