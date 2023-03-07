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

enum modes {NOTHING, COMMS, DEBUG};
int mode = DEBUG;

//Function definintons
uint32_t revAddrParser(uint32_t tag, uint32_t setID);
void addrParser(uint32_t address, uint16_t* tag, uint16_t* setID);
void command0(uint32_t address, Cache* cachePtr, uint16_t tag, uint16_t setID);
void command1(uint32_t address, Cache* cachePtr, uint16_t tag, uint16_t setID);
void command4(uint32_t address, Cache* cachePtr, uint16_t tag, uint16_t setID);

int main(int argc, char *argv[])
{	
	Cache dataL1(DATA_L1_WAYS, DATA_L1_SETS, DATA_L1_LINE_SIZE);
	Cache instL1(INST_L1_WAYS, INST_L1_SETS, INST_L1_LINE_SIZE);

	uint16_t tag = 0;
	uint16_t setID = 0;

	std::string filename = "test.txt";
	if(argc > 1){
		mode = atoi(argv[1]);
		std::cout << "Running in Mode: " << mode << std::endl;
		
		filename = argv[2];
	}	
	std::ifstream infile(filename);

	int command = 0;
	uint32_t address_hex = 0x0;

	std::string address_str = "";

	while(infile >> command >> address_str){
		address_hex = std::stoll(address_str, nullptr, 16);

		addrParser(address_hex, &tag, &setID);

		switch (command)
		{
		case 0:
			if (mode >= DEBUG)
			{
				std::cout << command << " " << std::hex << address_hex << std::dec << std::endl;
			}

			//Read from Data L1 cache
			command0(address_hex, &dataL1, tag, setID);
			break;
		case 1:
			if (mode >= DEBUG)
			{
				std::cout << command << " " << std::hex << address_hex << std::dec << std::endl;
			}

			//Write to Data L1 cache
			command1(address_hex, &dataL1, tag, setID);
			break;
		case 2:
			if (mode >= DEBUG)
			{
				std::cout << command << " " << std::hex << address_hex << std::dec << std::endl;
			}
			break;
		case 3:
			if (mode >= DEBUG)
			{
				std::cout << command << " " << std::hex << address_hex << std::dec << std::endl;
			}
			break;
		case 4:
			if (mode >= DEBUG)
			{
				std::cout << command << " " << std::hex << address_hex << std::dec << std::endl;
			}

			//Notice a request for data on the bus that is RFO
			command4(address_hex, &dataL1, tag, setID);
			break;
		case 8:
			if (mode >= DEBUG)
			{
				std::cout << "Reset Cache" << std::endl;
			}
			if(dataL1.resetCache()){
				if (mode >= DEBUG)
				{
					std::cout << "Data Cache Reset Complete" << std::endl;
				}
			}
			if(instL1.resetCache()){
				if (mode >= DEBUG)
				{
					std::cout << "Instruction Cache Reset Complete" << std::endl;
				}
			}
			break;
		case 9:
			if (mode >= DEBUG)
			{
				std::cout << command << " " << std::hex << address_hex << std::dec << std::endl;
			}
			break;
		default:
			std::cout << "\n\nERROR: Invalid command " << command << "\n\n";
			break;
		}
		dataL1.testPrintSet(14212);
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

	if (mode >= DEBUG)
	{
		std::cout << "Set: " << *setID << " | " << "Tag: " << *tag << std::endl;
	}
}

void command0(uint32_t address, Cache *cachePtr, uint16_t tag, uint16_t setID)
{
	bool isOccupiedAndModified = false;
	uint32_t evictedAddr = 0;

	//Get the line
	cacheLinePtr_t cacheLine = cachePtr->returnLine(tag, setID);
	
	//If the line isnt in the cache
	if (cacheLine == NULL)
	{
		//Log a miss
		dataL1Stats.misses += 1;

		//Find the next availble line
		cacheLine = cachePtr->getNextAvailLine(setID, &isOccupiedAndModified);

		//If the line we got was occupied with valid data, deal with it accordingly
		if (isOccupiedAndModified)
		{
			//Recreate the evicted address
			evictedAddr = revAddrParser(tag, setID);

			//Write evicted line back to L2
			if (mode >= COMMS)
			{
				std::cout << "\nWrite to L2 <" << std::hex << evictedAddr << std::dec << ">" << std::endl;
			}
		}

		//Retrieve our data form L2
		if (mode >= COMMS)
		{
			std::cout << "\nRead from L2 <" << std::hex << address << std::dec << ">" << std::endl;
		}

		//Write the new tag to L1
		cacheLine->tag = tag;

		//Mark line as exclusive
		cacheLine->MESI = EXCLUSIVE;

		//Warm the line if it isnt already
		if (cacheLine->isCold)
		{
			cacheLine->isCold = false;
		}
	}
	//Line exists but is invalid
	else if (cacheLine->MESI == INVALID)
	{
		//Log a miss
		dataL1Stats.misses += 1;

		//Retrieve from L2
		if (mode >= COMMS)
		{
			std::cout << "\nRead from L2 <" << std::hex << address << std::dec << ">" << std::endl;
		}

		//Mark line as exclusive
		cacheLine->MESI = EXCLUSIVE;
	}
	//Line existed in cache and line state is Exclusive OR Modified OR Shared
	else
	{
		if (cacheLine->MESI == EXCLUSIVE)
		{
			cacheLine->MESI = SHARED;
		}

		//Log a hit
		dataL1Stats.hits += 1;
	}

	//Update LRU bits
	cachePtr->updateLRU(cacheLine);

	//Log a read
	dataL1Stats.reads += 1;
}

void command1(uint32_t address, Cache* cachePtr, uint16_t tag, uint16_t setID)
{
	bool isOccupiedAndModified = false;
	uint32_t evictedAddr = 0;

	//Get the line
	cacheLinePtr_t cacheLine = cachePtr->returnLine(tag, setID);

	//If the line isnt in the cache
	if (cacheLine == NULL)
	{
		//Log a miss
		dataL1Stats.misses += 1;

		//Find the next availble line
		cacheLine = cachePtr->getNextAvailLine(setID, &isOccupiedAndModified);

		//If the line we got was occupied with valid data, deal with it accordingly
		if (isOccupiedAndModified)
		{
			//Recreate the evicted address
			evictedAddr = revAddrParser(cacheLine->tag, cacheLine->set);

			//Write evicted line back to L2
			if (mode >= COMMS)
			{
				std::cout << "\nWrite to L2 <" << std::hex << evictedAddr << std::dec << ">" << std::endl;
			}
		}

		//Retrieve our data form L2
		if (mode >= COMMS)
		{
			std::cout << "\nRead for Ownership from L2 <" << std::hex << address << std::dec << ">" << std::endl;
		}

		//Write the new tag to L1
		cacheLine->tag = tag;

		//Preform write though to L2
		if (cacheLine->isCold)
		{
			if (mode >= COMMS)
			{
				std::cout << "\nWrite to L2 <" << std::hex << address << std::dec << ">" << std::endl;
			}
		}

		//Mark line as exclusive
		cacheLine->MESI = MODIFIED;
	}
	else
	{
		switch (cacheLine->MESI)
		{
		case EXCLUSIVE || SHARED:
			//Write the new tag to L1
			cacheLine->tag = tag;

			//Mark line as exclusive
			cacheLine->MESI = MODIFIED;

			//Log a hit
			dataL1Stats.hits += 1;

			break;

		case MODIFIED:
			//Write the new tag to L1
			cacheLine->tag = tag;

			//Log a hit
			dataL1Stats.hits += 1;

			break;

		case INVALID:
			//Retrieve our data form L2
			if (mode >= COMMS)
			{
				std::cout << "\nRead for Ownership from L2 <" << std::hex << address << std::dec << ">" << std::endl;
			}

			//Write the new tag to L1
			cacheLine->tag = tag;

			//Mark line as exclusive
			cacheLine->MESI = MODIFIED;

			//Log a miss
			dataL1Stats.misses += 1;

			break;

		default:
			std::cout << "\nERROR: Invalid MESI state given\n" << std::endl;
			break;
		}
	}

	//Update LRU bits
	cachePtr->updateLRU(cacheLine);

	//Warm the line if it isnt already
	if (cacheLine->isCold)
	{
		cacheLine->isCold = false;
	}

	//Log a write
	dataL1Stats.writes += 1;
}

void command4(uint32_t address, Cache* cachePtr, uint16_t tag, uint16_t setID)
{
	//Get line
	cacheLinePtr_t cacheLine = cachePtr->returnLine(tag, setID);

	//If the line isnt in the cache
	if (cacheLine == NULL || cacheLine->MESI != MODIFIED)
	{
		//Do nothing, we have nothing to invalidate
		return;
	}
	else
	{
		//Return modified line to L2
		if (mode >= COMMS)
		{
			std::cout << "\nReturn data to L2  <" << std::hex << address << std::dec << ">" << std::endl;
		}

		//Mark line as Shared
		cacheLine->MESI = SHARED;

		//Mark line as Invalid
		cacheLine->MESI = INVALID; //Since this is a RFO, pretend the other processor had modified the value

		//Update LRU bits
		cachePtr->updateLRU(cacheLine);
	}
}

uint32_t revAddrParser(uint32_t tag, uint32_t setID)
{
	uint32_t address = 0;

	address = (tag << 20) | (setID << 6) | 0b000000;

	if (mode >= DEBUG)
	{
		std::cout << "Converted: " << std::hex << address << std::dec << std::endl;
	}

	return address;
}