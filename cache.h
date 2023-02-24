#pragma once

#include <cstdint>

#define NUM_OF_WAYS		4
#define NUM_OF_SETS		16000
#define LINE_SIZE		64 //In Bytes

#define LRU_INIT_STATE	0b00

typedef struct cacheLine
{
	uint8_t MESI;
	uint8_t LRU;
	uint16_t tag;
	uint16_t set;
} cacheLine_t, * cacheLinePtr_t;

enum MESI { MODIFIED, EXCLUSIVE, SHARED, INVALID };


class Cache
{
public:
	Cache();

	~Cache();

	cacheLinePtr_t returnLine(uint16_t tag, uint16_t setID);

	uint8_t returnMESI(uint16_t tag, uint16_t setID);

	bool updateLRU(cacheLinePtr_t lineAccessed);

private:

	void initialize();

	cacheLinePtr_t cacheSets[NUM_OF_SETS][NUM_OF_WAYS];
};

