#ifndef HELPER
#define HELPER

#include <iostream>

enum Collor{
  Red = 31,
  Green = 32
};

std::ostream &CStream( Collor a, std::ostream &b); 
std::ostream &ClrStream(std::ostream &b);


#endif
