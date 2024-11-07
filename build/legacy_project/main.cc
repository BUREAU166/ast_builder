#include "eval.h"
#include "adder.h"
#include <cstdio>

int main() {
  Adder adder(1);
  adder.increment();

  int val = eval(&adder);

  printf("val %d\n", val);

  return 0;
}
