#include <iostream>
#include "../include/unit.h"
#include "metrics.cc"

int main(int argc, char **argv) {
    parseUnit un(argv[1], argv[2]);
//    un.parse();
    //printf("unit %d", un.hi());

    std::vector<std::string> files = {"test/legacy.c", "test/recursion.c", "test/unary.c"};
    Metrics metrics(files);
    metrics.printMetrics();

    return 0;
}
