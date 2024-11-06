#include <iostream>
#include <string>
#include <unistd.h>

#include "../include/unit.h"
#include "metrics.cc"

void printUsage() {
    std::cout << "Usage:\n";
    std::cout << "  --file (-f) <filename>         Specify file for parsing\n";
    std::cout << "  --target (-t) <target>         Specify target for parsing (required for parsing mode)\n";
    std::cout << "  --metrics (-m)                 Collect code metrics\n";
}

int main(int argc, char **argv) {
    std::string file;
    std::string target;
    bool parseMode = false;
    bool metricsMode = false;

    int opt;
    while ((opt = getopt(argc, argv, "f:t:m")) != -1) {
        switch (opt) {
            case 'f':
                file = optarg;
                break;
            case 't':
                target = optarg;
                parseMode = true;
                break;
            case 'm':
                metricsMode = true;
                break;
            default:
                printUsage();
                return 1;
        }
    }

    if (file.empty()) {
        std::cerr << "Error: --file must be specified.\n";
        printUsage();
        return 1;
    }

    if (parseMode) {
        if (target.empty()) {
            std::cerr << "Error: --target must be specified for parsing.\n";
            printUsage();
            return 1;
        }

        parseUnit un(file, target);
        un.parse();
        std::cout << "Parsing file: " << file << " with target: " << target << "\n";
    }

    if (metricsMode) {
        Metrics metrics({file});
        metrics.printMetrics();
    }

    if (!parseMode && !metricsMode) {
        std::cerr << "Error: No operation specified.\n";
        printUsage();
        return 1;
    }

    return 0;
}
