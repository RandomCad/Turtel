#include <iostream>

#include "Lexer/Lexer.h"
#include "Helper.h"

int main(int argc, const char *argv[]){
  if(argc != 2){
    std::cerr << "Won argument is needed" << std::endl;
    return -1;
  }

  ClrStream(CStream(Collor::Green,std::cout) << "Start the Lexer!" << std::endl);
  ClrStream(CStream(Collor::Red,std::cout) << "Development reding from std::in" ) << "\nTesting lexer with std::in" << std::endl;
  
  Lex lx(argv[1]);
  std::cout << lx.getToken() << std::endl;
  
}
