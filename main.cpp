#include <iostream>
#include <string>
#include "cache.h"

#define INST_L1_WAYS		4
#define INST_L1_SETS		16000
#define INST_L1_LINE_SIZE	64

#define DATA_L1_WAYS		8
#define DATA_L1_SETS		16000
#define DATA_L1_LINE_SIZE	64

int main()
{	
	Cache dataL1(DATA_L1_WAYS, DATA_L1_SETS, DATA_L1_LINE_SIZE);
	Cache instL1(INST_L1_WAYS, INST_L1_SETS, INST_L1_LINE_SIZE);

	
	switch (command)
	{
	case 0:
		//Function 0 here
		break;
	case 1:
		//Function 1 here
		break;
	case 2:
		//Function 2 here
		break;
	case 3:
		//Funtion 3 here
		break;
	case 4:
		//Function 4 here
		break;
	case 8:
		//Function 8 here
		break;
	case 9:
		//Functoin 9 here
		break;
	default:
		std::cout << "\n\nERROR: Invalid command " << command << "\n\n";
		break;
	}

	//Hold terminal open until user exits
	std::cout << "Press <Enter> to close..." << std::endl;
	std::cin.get();

	return 0;
}