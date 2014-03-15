/* *****************************************************
 *	main.cpp - Jijidici - 15/03/2014
 * ***************************************************** */

#include <cstdlib>
#include <iostream>
#include "ScreenBox.h"

int main(int argc, char** argv) {
	std::cout << "> PROGRAM LAUNCH" << std::endl;

	ScreenBox sb;
	sb.init();
	sb.launch();
	sb.destroy();

	std::cout << "> PROGRAM EXIT" << std::endl;
	return EXIT_SUCCESS;
}