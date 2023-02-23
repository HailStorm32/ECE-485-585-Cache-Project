#pragma once

#include <cstdint>

#define NUM_OF_WAYS		4
#define NUM_OF_SETS		16000
#define LINE_SIZE		64 //In Bytes

class Cache
{
public:
	Cache();

	~Cache();

private:

	void initialize();

	enum MESI {MODIFIED, EXCLUSIVE, SHARED, INVALID};

	typedef struct cacheLine
	{
		uint8_t MESI;
		uint8_t LRU;
		uint16_t tag;
	} cacheLine_t, *cacheLinePtr_t;

	cacheLinePtr_t cacheSets[NUM_OF_SETS][NUM_OF_WAYS];
};

