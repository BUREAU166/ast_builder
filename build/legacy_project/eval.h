#ifndef EVAL_H
#define EVAL_H

#include <vector>
#include "adder.h"
#include <algorithm>

class Dummy {
public:
  int x;

  Dummy(int _x) {
    x = _x;
  }
};

int eval(void* arg);

bool isEven(void* arg);

std::vector<Adder> filter(std::vector<Adder> adders);

#endif 
