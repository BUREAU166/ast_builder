#!/bin/bash

# clang++-14 -o ast -I/usr/lib/llvm-14/include  -include ../include/errorLoc.h ../src/errorLoc.cc ../src/main.cc  -L/lib/x86_64-linux-gnu -lclang-14

clang++-14 -o errorLoc.o -include ../include/errorLoc.h -c ../src/errorLoc.cc

clang++-14 -o visitorData.o -include ../include/visitorData.h -c ../src/visitorData.cc

clang++-14 -v -o unit.o -I/usr/lib/llvm-14/include -include ../include/unit.h -c ../src/unit.cc

clang++-14 -v -o main.o -I/usr/lib/llvm-14/include -I${PWD}/../include -c ../src/main.cc

clang++-14 -v -o metrics.o -I/usr/lib/llvm-14/include -include ../include/metrics.h -c ../src/metrics.cc

clang++-14 -o ast errorLoc.o visitorData.o unit.o main.o -L/lib/x86_64-linux-gnu -lclang-14

