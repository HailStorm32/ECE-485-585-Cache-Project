#include "CommandLine_Parser.h"

void CommandLineParser::processInputFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: could not open input file " << filename << std::endl;
        return;
    }

    std::string line;
    uint32_t reads = 0, writes = 0, hits = 0, misses = 0;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        char access_type;
        uint32_t address;
        if (!(iss >> access_type >> std::hex >> address)) {
            std::cerr << "Warning: skipping invalid input line: " << line << std::endl;
            continue;
        }

        switch (access_type) {
        case 'R':
            accessInstructionCache(address);
            ++reads;
            break;
        case 'W':
            accessDataCache(address);
            ++writes;
            break;
        default:
            std::cerr << "Warning: skipping unrecognized access type '" << access_type
                << "' in input line: " << line << std::endl;
            continue;
        }

        if (last_access_hit_) {
            ++hits;
        }
        else {
            ++misses;
        }
    }

    double hit_ratio = 0.0;
    if (reads + writes > 0) {
        hit_ratio = static_cast<double>(hits) / (reads + writes);
    }

    std::cout << "Reads: " << reads << std::endl;
    std::cout << "Writes: " << writes << std::endl;
    std::cout << "Cache hits: " << hits << std::endl;
    std::cout << "Cache misses: " << misses << std::endl;
    std::cout << "Hit ratio: " << hit_ratio << std::endl;
}

