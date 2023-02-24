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
	line->MESI = MODIFIED;
	S1.updateLRU(line);

	line = S1.returnLine(0, 1);

	line->tag = 500;
	S1.updateLRU(line);

	S1.testPrintSet(0);
	S1.testPrintSet(1);

	line = S1.getNextAvailLine(0);

	line->tag = 400;
	S1.updateLRU(line);

	S1.testPrintSet(0);
	S1.testPrintSet(1);

	std::cout << "HELLO" << std::endl;

	std::cin >> test;

	return 0;
}