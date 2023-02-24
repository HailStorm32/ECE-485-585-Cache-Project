#include <iostream>
#include <string>
#include "cache.h"

int main()
{
	//Below is just testing code, feel free to delete
	std::string test;

	Cache S1(4, 16000, 64);

	cacheLinePtr_t line = S1.returnLine(0, 0);

	line->tag = 323;

	line = S1.returnLine(0, 1);

	line->tag = 500;

	line = S1.returnLine(323, 0);

	std::cout << "TEST: " << (int)line->MESI << " | " << (int)line->tag << std::endl;

	line = S1.returnLine(500, 1);

	std::cout << "TEST: " << (int)line->MESI << " | " << (int)line->tag << std::endl;

	std::cout << "HELLO" << std::endl;

	std::cin >> test;

	return 0;
}