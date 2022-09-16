#ifndef CONFIG_H
#define CONFIG_H
#pragma once
#include <string>

namespace sig {

    struct Config {
        std::string inputFile;
        std::string outputFile;
        size_t blockSize = 0;
        int threadNum = 0;
        int workerNum = 0;
    };

}  // namespace sig

#endif