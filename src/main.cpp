/* *****************************************************
 *	main.cpp - Jijidici - 15/03/2014
 * ***************************************************** */

#include <cstdlib>
#include <iostream>
#include "ScreenBox.h"

int main(int argc, char** argv) {
	std::cout << "> PROGRAM LAUNCH" << std::endl;

	ScreenBox* sb = new ScreenBox();
	sb->init();
	sb->launch();
	sb->destroy();
	delete sb;

	std::cout << "> PROGRAM EXIT" << std::endl;
	return EXIT_SUCCESS;
}