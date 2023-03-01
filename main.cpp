#include <iostream>
#include <string>
#include <fstream>
#include "cache.h"

#define INST_L1_WAYS		4
#define INST_L1_SETS		16000
#define INST_L1_LINE_SIZE	64

#define DATA_L1_WAYS		8
#define DATA_L1_SETS		16000
#define DATA_L1_LINE_SIZE	64

struct cacheStats 
{
	uint16_t hits;
	uint16_t misses;
	uint16_t writes;
	uint16_t reads;
} dataL1Stats, instL1Stats;

//Function definintons
void addrParser(uint32_t address, uint16_t* tag, uint16_t* setID);
void command0(uint32_t address, Cache* cachePtr, uint16_t tag, uint16_t setID);

int main(int argc, char *argv[])
{	
	Cache dataL1(DATA_L1_WAYS, DATA_L1_SETS, DATA_L1_LINE_SIZE);
	Cache instL1(INST_L1_WAYS, INST_L1_SETS, INST_L1_LINE_SIZE);

	uint16_t tag = 0;
	uint16_t setID = 0;

	int mode = 0;
	std::string filename = "test.txt";
	if(argc > 1){
		mode = atoi(argv[1]);
		std::cout << "Running in Mode: " << mode << std::endl;
		
		filename = argv[2];
	}	
	std::ifstream infile(filename);

	int command = 0, address_hex = 0x0;
	std::string address_str = "";

	while(infile >> command >> address_str){
		address_hex = std::stoi(address_str, 0, 16);

		addrParser(address_hex, &tag, &setID);

		switch (command)
		{
		case 0:
			std::cout << command << " " << address_hex << std::endl;

			//Read from Data L1 cache
			command0(address_hex, &dataL1, tag, setID);
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

void command0(uint32_t address, Cache *cachePtr, uint16_t tag, uint16_t setID)
{
	bool isOccupiedAndValid = false;
	uint32_t evictedAddr = 0;

	//Get the line
	cacheLinePtr_t cacheLine = cachePtr->returnLine(tag, setID);
	
	//If the line isnt in the cache
	if (cacheLine == NULL)
	{
		//Log a miss
		dataL1Stats.misses += 1;

		//Find the next availble line
		cacheLine = cachePtr->getNextAvailLine(setID, &isOccupiedAndValid);

		//If the line we got was occupied with valid data, deal with it accordingly
		if (isOccupiedAndValid)
		{
			//TODO: recreate the evicted address

			//Write evicted line back to L2
			std::cout << "\nWrite to L2 <" << std::hex << evictedAddr << std::dec << ">" << std::endl;

			//Retrieve our data form L2
			std::cout << "\nRead from L2 <" << std::hex << address << std::dec << ">" << std::endl;
		}

		//Write the new tag to L1
		cacheLine->tag = tag;

		//Mark line as exclusive
		cacheLine->MESI = EXCLUSIVE;
	}
	//Line exists but is invalid
	else if (cacheLine->MESI == INVALID)
	{
		//Log a miss
		dataL1Stats.misses += 1;

		//Retrieve from L2
		std::cout << "\nRead from L2 <" << std::hex << address << std::dec << ">" << std::endl;

		//Mark line as exclusive
		cacheLine->MESI = EXCLUSIVE;
	}
	//Line existed in cache and line state is Exclusive OR Modified OR Shared
	else
	{
		//Log a hit
		dataL1Stats.hits += 1;
	}

	//Update LRU bits
	cachePtr->updateLRU(cacheLine);

	//Log a read
	dataL1Stats.reads += 1;
}