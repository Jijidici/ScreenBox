/* *****************************************************
 *	MouseHandling.cpp - Jijidici - 16/03/2014
 * ***************************************************** */
#include "MouseHandling.h"

MouseHandling* MouseHandling::_pInstance = nullptr;

MouseHandling::MouseHandling() {

}

MouseHandling* MouseHandling::getInstance() {
	if(_pInstance == nullptr) {
		_pInstance = new MouseHandling();
	}
	return _pInstance;
}