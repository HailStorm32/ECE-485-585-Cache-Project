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

void addrParser(uint32_t address, uint16_t* tag, uint16_t* setID);

int main()
{	
	Cache dataL1(DATA_L1_WAYS, DATA_L1_SETS, DATA_L1_LINE_SIZE);
	Cache instL1(INST_L1_WAYS, INST_L1_SETS, INST_L1_LINE_SIZE);

	uint16_t tag = 0;
	uint16_t setID = 0;

	

	std::ifstream infile("test.txt");

	int command = 0, address_hex = 0x0;
	std::string address_str = "";

	while(infile >> command >> address_str){
		address_hex = std::stoi(address_str, 0, 16);

		addrParser(address_hex, &tag, &setID);

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

void addrParser(uint32_t address, uint16_t *tag, uint16_t *setID)
{
	//Make sure the varibles we are using for output are "clean"
	*tag = 0;
	*setID = 0;

	//Mask bits [31:20] and [5:0] then right shift 6bits to get set
	*setID = (address & 0xFFFC0) >> 6;

	//Mask bits [19:6] and [5:0] then right shift 20bits to get tag
	*tag = (address & 0xFFF00000) >> 20;

	std::cout << "Set: " << *setID << " | " << "Tag: " << *tag << std::endl;
}