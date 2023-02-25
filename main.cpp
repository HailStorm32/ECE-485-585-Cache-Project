#include <iostream>
#include <string>
#include <fstream>
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

	std::ifstream infile("test.txt");

	int command = 0, address_hex = 0x0;
	std::string address_str = "";

	while(infile >> command >> address_str){
		address_hex = std::stoi(address_str, 0, 16);
		switch (command)
		{
		case 0:
			std::cout << command << " " << address_hex << std::endl;
			break;
		case 1:
			std::cout << command << " " << address_hex << std::endl;
			break;
		case 2:
			std::cout << command << " " << address_hex << std::endl;
			break;
		case 3:
			std::cout << command << " " << address_hex << std::endl;
			break;
		case 4:
			std::cout << command << " " << address_hex << std::endl;
			break;
		case 8:
			std::cout << command << " " << address_hex << std::endl;
			break;
		case 9:
			std::cout << command << " " << address_hex << std::endl;
			break;
		default:
			std::cout << "\n\nERROR: Invalid command " << command << "\n\n";
			break;
		}
	}

	//Hold terminal open until user exits
	std::cout << "Press <Enter> to close..." << std::endl;
	std::cin.get();

	return 0;
}