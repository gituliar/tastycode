//
//  MIT License
//
//  Copyright (c) 2024 Oleksandr Gituliar
//
//  TastyCode - https://tastycode.dev
//
#include <iostream>

#include "tracy/Tracy.hpp"

#include "TextFileReader.h"


//  Comment this line to enable Tracy profiler
//
#undef TRACY_ENABLE


Error
chatGptReadline(const fs::path& srcPath, u64& avgLineLength)
{
    ZoneScoped;

    std::ifstream file(srcPath.string()); // Open the file

    if (file.is_open()) { // Check if the file is opened successfully
        avgLineLength = 0;
        u64 n = 0;

        std::string line;
        while (std::getline(file, line)) { // Read each line of the file
            avgLineLength += line.size();
            n++;
        }

        avgLineLength /= n;

        file.close(); // Close the file when done
    }
    else {
        // Print error message if file cannot be opened
        return "chatGptReadline: Unable to open the file";
    }

    return "";
}

Error
tastyReadline(const fs::path& srcPath, u64& avgLineLength) {
    ZoneScoped;

    tasty::TextFileReader txt;
    if (auto err = txt.open(srcPath); !err.empty())
        return "tastyReadline: " + err;

    u64 n = 0;
    Error err;
    while (txt.readline(err)) {
        avgLineLength += txt.line().size();
        n++;
    }

    return "";
}


i32 main(i32 argc, char** argv)
{
    for (auto i = 1; i < argc; i++) {
        fs::path srcPath(argv[i]);

        for (auto i = 0; i < 200; i++) {
            u64 n;

            std::cout << "tastyReadline: " << srcPath << std::endl;
            if (auto err = tastyReadline(srcPath, n); !err.empty()) {
                std::cerr << err;
                return -1;
            }

            std::cout << "chatGptReadline: " << srcPath << std::endl;
            if (auto err = chatGptReadline(srcPath, n); !err.empty()) {
                std::cerr << err;
                return -1;
            }
        }
    }

    return 0;
}
