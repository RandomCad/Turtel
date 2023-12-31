#ifndef LEXER
#define LEXER

#include <ostream>
#include <fstream>
#include <vector>
#include <iostream>

#include "Token.h"
#include "LexerError.h"

#define READ err.AddChar((char)st.get())

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
    void swapStream(std::ifstream &a);
    void swapStream(const char *a);
    #endif

  private:
    Token *GetToken(LexErr &err);
    friend void TestRemoveWhites();
    
    Token *RWalk(LexErr &err);
    friend void TestLexWalk();

    Token *RWalkBack();

    Token *RInt();

    LexErr &ConsumTerm(char a, LexErr &err);
    friend void TestConsumTerm();

    LexErr &TryConsumTerm(char a, LexErr &err);
    friend void TestTryConsumTerm();

  template <int (*Condition)(int)>
  LexErr &ConsumTerm(char a, LexErr &err){
    if(st.peek() == a ||Condition(st.peek())){
      READ;
      return err;
    }
    //error
    return err.ConsumedWrong(a);
  }
  friend void TestConsumTermTemplate();

    /*Furture use
    template<char C>
    LexErr &ConsumTerm(char a, LexErr &err){
      
    }//*/

};

#ifndef LEXER_CPP
#undef READ
#endif
#endif
