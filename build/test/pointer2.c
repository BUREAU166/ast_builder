#include <stdio.h>

int main() {
  char b = 'x';

  void* vp = &b;

  char a = *(char*)vp;

  printf("a is %c", a);
}
