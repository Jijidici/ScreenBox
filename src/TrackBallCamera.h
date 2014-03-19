/* *****************************************************
 *	TrackBallCamera.h - Jijidici - 16/03/2014
 * ***************************************************** */
#ifndef __TRACKBALL_CAMERA_H__
#define __TRACKBALL_CAMERA_H__

#include "glm/glm.hpp"

class TrackBallCamera{
public:
	static TrackBallCamera* getInstance();
	void moveFront(float distance);
	void rotateLeft(float degrees);
	void rotateUp(float degrees);
	glm::mat4 getViewMatrix();
	glm::vec3 getCameraPosition() const;
	float getAngleX() { return m_fAngleX; }
	float getAngleY() { return m_fAngleY; }

private:
	TrackBallCamera();

	static TrackBallCamera* _pInstance;
	float m_fDistance;
	float m_fAngleX;
	float m_fAngleY;
	glm::vec3 m_fPosition;
};

#endif //__TRACKBALL_CAMERA_H__