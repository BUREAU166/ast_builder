#ifndef ADDER_H
#define ADDER_H

class Adder {
public:
  int val;
  Adder(int v) {
    val = v;
  }

  int increment();

  int decrement(); 
};

#endif
