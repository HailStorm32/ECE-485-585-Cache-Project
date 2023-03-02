#include "cache.h"
#include <iostream>

Cache::Cache(uint8_t numOfWays, uint16_t numOfSets, uint8_t lineSize)
{
	this->numOfWays = numOfWays;
	this->numOfSets = numOfSets;
	this->lineSize = lineSize;

	//Create array to hold cache
	cacheSets = new cacheLinePtr_t* [numOfSets];
	for (int index = 0; index < numOfSets; index++) 
	{
		cacheSets[index] = new cacheLinePtr_t[numOfWays];
	}

	//Set the max state for LRU
	lruMaxState = numOfWays - 1;

	Cache::initialize();
}

Cache::~Cache()
{
	for (int setIndex = 0; setIndex < numOfSets; setIndex++)
	{
		for (int lineIndex = 0; lineIndex < numOfWays; lineIndex++)
		{
			if (cacheSets[setIndex][lineIndex] != NULL)
			{
				delete cacheSets[setIndex][lineIndex];
			}	
		}
		delete cacheSets[setIndex];
	}
}

/*
* Description: 
	Find and return line in cache given the set and tag.
*	Will a pointer to that line's struct, else will return NULL
* 
* Arguments:
*	(INPUT) tag -- 16bit tag for the line you want to find
*	(INPUT) setID -- 14bit value representing the set the line is in
* 
* Return:
*	cacheLinePtr_t -- Pointer to line struct if line found
*	NULL -- If line is not found
*/
cacheLinePtr_t Cache::returnLine(uint16_t tag, uint16_t setID)
{
	//Cycle through the lines in the set
	for (int lineIndex = 0; lineIndex < numOfWays; lineIndex++) 
	{
		//Find a matching tag
		if (cacheSets[setID][lineIndex]->tag == tag)
		{
			return cacheSets[setID][lineIndex];
		}
	}

	//If we cant find the line, return NULL
	return NULL;
}

/*
* Description:
*	Returns the MESI bits for a given line in a set
* 
* Arguments:
*	(INPUT) tag -- 16bit tag for the line you want to find
*	(INPUT) setID -- 14bit value representing the set the line is in
* 
* Return:
*	MESIbits -- enum of MESI state
*/
MESIbits Cache::returnMESI(uint16_t tag, uint16_t setID)
{
	cacheLinePtr_t line = Cache::returnLine(tag, setID);

	if (line != NULL)
	{
		return line->MESI;
	}
	else
	{
		return ERROR;
	}
}

/*
* Description:
*	Update all the LRU bits in the set that the line we accessed is a part of
* 
* Arguments:
*	(INPUT) lineAccessed -- pointer to the line that was accessed
* 
* Return:
*	true -- LRU bits updated successfully
*	false -- failed to update LRU bits
*/
bool Cache::updateLRU(cacheLinePtr_t lineAccessed)
{
	if (lineAccessed != NULL)
	{
		for (int lineIndex = 0; lineIndex < numOfWays; lineIndex++)
		{
			if (cacheSets[lineAccessed->set][lineIndex]->LRU <= lineAccessed->LRU && cacheSets[lineAccessed->set][lineIndex]->LRU != lruMaxState)
			{
				cacheSets[lineAccessed->set][lineIndex]->LRU++;
			}
		}

		lineAccessed->LRU = LRU_INIT_STATE;

		return true;
	}
	else
	{
		return false;
	}
}

/*
* Description:
*	Retrieve the next line in a set. Looks for the first unoccupied/invalid line. 
*	If it cant find that, it will return the LRU line
* 
* Arguments:
*	(INPUT) setID -- set index you want to look in
*	(OUTPUT) isOccupied -- indicates if the line found was occupied by valid data
* 
* Returns:
*	cacheLinePtr_t -- pointer to line struct
*/
cacheLinePtr_t Cache::getNextAvailLine(uint16_t setID, bool *isOccupiedAndMod)
{
	int highestLRUIndex = INT16_MIN;
	int highestLRUval = INT16_MIN;

	//Look for the first invalid line
	for (int lineIndex = 0; lineIndex < numOfWays; lineIndex++)
	{
		if (cacheSets[setID][lineIndex]->MESI == INVALID)
		{
			*isOccupiedAndMod = false;
			return cacheSets[setID][lineIndex];
		}
	}

	//If we get this far, all lines are occupied and valid
	//Next, find the LRU line
	for (int lineIndex = 0; lineIndex < numOfWays; lineIndex++)
	{
		if (cacheSets[setID][lineIndex]->LRU > highestLRUval)
		{
			highestLRUval = cacheSets[setID][lineIndex]->LRU;
			highestLRUIndex = lineIndex;
		}
	}
	
	//Log if the line has been modified
	if (cacheSets[setID][highestLRUIndex]->MESI == MODIFIED)
	{
		*isOccupiedAndMod = true;
	}

	return cacheSets[setID][highestLRUIndex];
}

void Cache::testPrintSet(uint16_t setID)
{
	for (int lineIndex = 0; lineIndex < numOfWays; lineIndex++)
	{
		std::cout << lineIndex << ": MESI: " << cacheSets[setID][lineIndex]->MESI << " | LRU: " << (int)cacheSets[setID][lineIndex]->LRU << " | TAG: " << cacheSets[setID][lineIndex]->tag << std::endl;
	}
	std::cout << std::endl;
}

/*
* Description: 
	Initialize the datatypes that make up the cache to inital values
* 
* Arguments:
*	NONE
* 
* Return:
*	NONE
*/
void Cache::initialize()
{
	//Cycle through all the sets
	for (int setIndex = 0; setIndex < numOfSets; setIndex++)
	{
		//Cycle through lines in set
		for (int lineIndex = 0; lineIndex < numOfWays; lineIndex++)
		{
			cacheLinePtr_t linePtr = new cacheLine_t; //(cacheLinePtr_t)malloc(sizeof(cacheLine_t));

			if (linePtr == NULL)
			{
				std::cout << "\nERROR: Cant allocate memory for linePtr\n" << std::endl;
				return;
			}

			linePtr->MESI = INVALID;
			linePtr->LRU = LRU_INIT_STATE;
			linePtr->tag = 0;
			linePtr->set = setIndex;

			cacheSets[setIndex][lineIndex] = linePtr;
		}
	}
}
