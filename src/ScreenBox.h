/* *****************************************************
 *	ScreenBox.h - Jijidici - 15/03/2014
 * ***************************************************** */

#ifndef __SCREEN_BOX_H__
#define __SCREEN_BOX_H__

class ScreenBox {
public:
	void init();
	void launch();
	void destroy();

private:
	int _iW;
	int _iH;
	bool _bLoop;
};

#endif //__SCREEN_BOX_H__