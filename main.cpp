#include <iostream>

#include "Lexer/Lexer.h"
#include "Helper.h"

#ifdef U_TEST
int TestMain(int argc, const char * argv[]){
#else
int main(int argc, const char *argv[]){
#endif

  if(argc != 2){
    std::cerr << "Won argument is needed" << std::endl;
    return -1;
  }

  ClrStream(CStream(Collor::Green,std::cout) << "Start the Lexer!" << std::endl);
  ClrStream(CStream(Collor::Red,std::cout) << "Development reding from std::in" ) << "\nTesting lexer with std::in" << std::endl;
  
  Lex lx(argv[1]);
  
}
