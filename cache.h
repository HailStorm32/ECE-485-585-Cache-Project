#pragma once

#include <cstdint>

#define LRU_INIT_STATE	0b00


enum MESIbits { MODIFIED, EXCLUSIVE, SHARED, INVALID , ERROR};

typedef struct cacheLine
{
	MESIbits MESI;
	uint8_t LRU;
	uint16_t tag;
	uint16_t set;
} cacheLine_t, *cacheLinePtr_t;


class Cache
{
public:
	Cache(uint8_t numOfWays, uint16_t numOfSets, uint8_t lineSize);

	~Cache();

	cacheLinePtr_t returnLine(uint16_t tag, uint16_t setID);

	MESIbits returnMESI(uint16_t tag, uint16_t setID);

	bool updateLRU(cacheLinePtr_t lineAccessed);

	cacheLinePtr_t getNextAvailLine(uint16_t setID);

	void testPrintSet(uint16_t setID);

private:

	void initialize();

	cacheLinePtr_t **cacheSets;

	uint8_t numOfWays;
	uint16_t numOfSets;
	uint8_t lineSize; //In Bytes
	uint8_t lruMaxState;
};

