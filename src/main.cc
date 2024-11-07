#include <iostream>
#include "../include/unit.h"

int main( int argc, char** argv )
{
  if(argc < 3) {
    parseUnit un(argv[1]);
    un.parse();
  }
  else {
    parseUnit un(argv[1], argv[2]);
    un.parse();
  }
    //printf("unit %d", un.hi());
  return 0;
}
