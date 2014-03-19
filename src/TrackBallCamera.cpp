/* *****************************************************
 *	TrackBallCamera.cpp - Jijidici - 16/03/2014
 * ***************************************************** */
#include "TrackBallCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

TrackBallCamera* TrackBallCamera::_pInstance = nullptr;

TrackBallCamera::TrackBallCamera() 
	: m_fDistance(-5.f)
	, m_fAngleX(0.f)
	, m_fAngleY(0.f)
	, m_fPosition(0.f){

}

TrackBallCamera* TrackBallCamera::getInstance() {
	if(_pInstance == nullptr) {
		_pInstance = new TrackBallCamera();
	}
	return _pInstance;
}

void TrackBallCamera::moveFront(float distance){
	m_fDistance -= distance;
	if(m_fDistance > -0.5f){
		m_fDistance = -0.5f;
	}
}

void TrackBallCamera::rotateLeft(float degrees){
	m_fAngleY += degrees;
}

void TrackBallCamera::rotateUp(float degrees){
	float nextAngle = m_fAngleX + degrees;
	if(nextAngle >= -175 && nextAngle <= 175){
		m_fAngleX = nextAngle;
	}
}

glm::mat4 TrackBallCamera::getViewMatrix(){
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.f,0.f,m_fDistance), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f,1.f,0.f));
	viewMatrix = glm::rotate(viewMatrix, m_fAngleX, glm::vec3(1.f, 0.f, 0.f));
	viewMatrix = glm::rotate(viewMatrix, m_fAngleY, glm::vec3(0.f, 1.f, 0.f));

	//update cam position
	glm::vec4 camPosition = (glm::inverse(viewMatrix)*glm::vec4(0.f, 0.f, 0.f, 1.f));
	m_fPosition = glm::vec3(camPosition.x, camPosition.y, camPosition.z);

	return viewMatrix;
}

glm::vec3 TrackBallCamera::getCameraPosition() const{
	return m_fPosition;
}