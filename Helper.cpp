#include "Helper.h"

#include <iostream>

std::ostream &CStream(Collor a, std::ostream &b){
  return b << "\033[" << a << 'm';
}

std::ostream &ClrStream(std::ostream &b){
  return b << "\033[0m";
}

