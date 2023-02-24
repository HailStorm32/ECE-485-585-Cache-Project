#include "cache.h"
#include <malloc.h>
#include <iostream>

Cache::Cache()
{
	Cache::initialize();
}

Cache::~Cache()
{
	for (int setIndex = 0; setIndex < NUM_OF_SETS; setIndex++)
	{
		for (int lineIndex = 0; lineIndex < NUM_OF_WAYS; lineIndex++)
		{
			if (cacheSets[setIndex][lineIndex] != NULL)
			{
				free(cacheSets[setIndex][lineIndex]);
			}		
		}
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
	for (int lineIndex = 0; lineIndex < NUM_OF_WAYS; lineIndex++) 
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


uint8_t Cache::returnMESI(uint16_t tag, uint16_t setID)
{
	cacheLinePtr_t line = Cache::returnLine(tag, setID);

	if (line != NULL)
	{
		return line->MESI;
	}
	else
	{
		return 255;
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
		for (int lineIndex = 0; lineIndex < NUM_OF_WAYS; lineIndex++)
		{
			if (cacheSets[lineAccessed->set][lineIndex]->LRU < lineAccessed->LRU)
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
	for (int setIndex = 0; setIndex < NUM_OF_SETS; setIndex++)
	{
		//Cycle through lines in set
		for (int lineIndex = 0; lineIndex < NUM_OF_WAYS; lineIndex++)
		{
			cacheLinePtr_t linePtr = (cacheLinePtr_t)malloc(sizeof(cacheLine_t));

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
