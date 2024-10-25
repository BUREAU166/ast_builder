#include <iostream>
#include "../include/unit.h"

int main( int argc, char** argv )
{
  parseUnit un(argv[1]);
  un.parse();
  //printf("unit %d", un.hi());
  return 0;
}
