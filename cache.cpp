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

void Cache::initialize()
{
	for (int setIndex = 0; setIndex < NUM_OF_SETS; setIndex++)
	{
		for (int lineIndex = 0; lineIndex < NUM_OF_WAYS; lineIndex++)
		{
			cacheLinePtr_t linePtr = (cacheLinePtr_t)malloc(sizeof(cacheLine_t));

			if (linePtr == NULL)
			{
				std::cout << "\nERROR: cant allocate memory for linePtr\n" << std::endl;
				return;
			}

			linePtr->MESI = INVALID;
			linePtr->LRU = 0;
			linePtr->tag = 0;

			cacheSets[setIndex][lineIndex] = linePtr;
		}
	}
}
