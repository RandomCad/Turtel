#ifndef HELPER
#define HELPER

#include <iostream>

#define RED \033[31m
#define GREEN \033[32m
#define CLEAR \033[0m

enum Collor{
  Red = 31,
  Green = 32
};

std::ostream &CStream( Collor a, std::ostream &b); 
std::ostream &ClrStream(std::ostream &b);


#endif
