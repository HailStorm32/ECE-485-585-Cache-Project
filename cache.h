#pragma once

#include <cstdint>

#define LRU_INIT_STATE	0b00

typedef struct cacheLine
{
	uint8_t MESI;
	uint8_t LRU;
	uint16_t tag;
	uint16_t set;
} cacheLine_t, *cacheLinePtr_t;

enum MESI { MODIFIED, EXCLUSIVE, SHARED, INVALID };


class Cache
{
public:
	Cache(uint8_t numOfWays, uint16_t numOfSets, uint8_t lineSize);

	~Cache();

	cacheLinePtr_t returnLine(uint16_t tag, uint16_t setID);

	uint8_t returnMESI(uint16_t tag, uint16_t setID);

	bool updateLRU(cacheLinePtr_t lineAccessed);

private:

	void initialize();

	cacheLinePtr_t **cacheSets;

	uint8_t numOfWays;
	uint16_t numOfSets;
	uint8_t lineSize; //In Bytes
};

