#pragma once

#ifndef COMMANDLINE_PARSER_H
#define COMMANDLINE_PARSER_H

#include <cstdint>
#include <vector>
#include <cmath>

class CommandLineParser {
public:
    CommandLineParser(uint32_t l1_instr_cache_size, uint32_t l1_data_cache_size,
        uint32_t l2_cache_size, uint32_t line_size);
    void accessMemory(uint32_t address, bool is_write);

private:
    struct CacheLine {
        uint32_t tag;
        bool valid;
        bool dirty;
        uint64_t lru;
    };

    uint32_t l1_instr_cache_size_;
    uint32_t l1_instr_cache_index_bits_;
    uint32_t l1_data_cache_size_;
    uint32_t l1_data_cache_index_bits_;
    uint32_t l2_cache_size_;
    uint32_t l2_cache_index_bits_;
    uint32_t line_size_;

    std::vector<std::vector<CacheLine>> l1_instr_cache_;
    std::vector<std::vector<CacheLine>> l1_data_cache_;
    std::vector<CacheLine> l2_cache_;

    void accessCacheLine(std::vector<CacheLine>& cache, uint32_t index,
        bool evict);
    uint32_t findLRULine(std::vector<CacheLine>& cache);
};

#endif  // COMMANDLINE_PARSER_H

