#ifndef LEXER
#define LEXER

#include <ostream>
#include <fstream>
#include <vector>
#include <iostream>

#ifdef DEBUG
#include <iostream>
#endif

#include "Token.h"

//main Lexer class
class Lex{
  private:
    std::vector<Token> tokSt;
    std::string sb;
    //should be on the head moust be deleted in the destrucktor. The Lexer takes the ownership!
    std::ifstream st;
  public:
    //takes ownership of the stream. It shudn't be used outside!
    Lex(const char *St) : st(St) {
      #ifdef DEBUG
      std::cout << "creating new Lex with: " << St << std::endl;
      #endif
      if (!st.is_open()){
        std::cerr << "couldn't open file: " << St << std::endl;
      }
    }
    //TODO: check correct opening!
    //TODO: dtor!
    
    #ifdef DEBUG
    Token *getToken() { return GetToken(); }
    #endif
    #ifdef U_TEST
    Token *TestGetToken() { return GetToken(); }
    void swapStream(std::ifstream &a);
    void swapStream(const char *a);
    #endif

  private:
    Token *GetToken();
    
    Token *RWalk();
    Token *RWalkBack();

    Token *RInt();

    bool ConsumTerm(char a);
    friend void TestConsumTerm();

};
#endif
