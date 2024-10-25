#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>


atomic_int p;

int main() {
  int hmod = abs(-1);
  
  bool hflag = true;
  
  uint8_t huint = 12;

  p++;

  return 0;
}
