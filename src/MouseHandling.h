/* *****************************************************
 *	MouseHandling.h - Jijidici - 16/03/2014
 * ***************************************************** */
#ifndef __MOUSE_HANDLING_H__
#define __MOUSE_HANDLING_H__

struct MouseHandling {
public:
	bool bLeftMousePressed;
	double savedPosX;
	double savedPosY;
	float fGamma;
	float fSpeed;
	int selectedFilters[2];

	static MouseHandling* getInstance();

private:
	MouseHandling();
	static MouseHandling* _pInstance;
};
#endif //__MOUSE_HANDLING_H__