#ifndef LEXER
#define LEXER

#include <ostream>
#include <fstream>
#include <vector>

#include "Token.h"

//main Lexer class
class Lex: private std::istream{
  private:
    std::vector<Token> tokSt;
    std::string sb;
  public:

  private:
    Token *GetToken();
    
    Token *RWalk();
    Token *RWalkBack();

    Token *RInt();

    bool ConsumTerm(char a);

};
#endif
