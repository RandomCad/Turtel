#define LEXER_CPP
#include "Token.h"
#include "Lexer.h"
#include "LexerError.h"

#include <cctype>
#include <fstream>
#include <iostream>

std::ostream &operator<< (std::ostream &a, Token &b){
  return a << "List pointer (prev,next): " << b.prev << ',' << b.next << "\nToken str: " << b.str << "\nToken type: " << b.tok << std::endl;
}


Token *Lex::GetToken(LexErr &err){
  #ifdef DEBUG
  std::cout << "Calling GetToken" << std::endl;
  #endif
  /*
  do{
    switch (st.peek()){
      case std::char_traits<char>::eof():
        #ifdef DEBUG
        std::cout << "EOF" << std::endl;
        #endif
        err.EOFReached();
        return nullptr;
      case ' ':
      case '\t':
      case '\n':
      case '\v':
      case '\f':
      case '\r':
        READ;
        continue;
          
      //"walk " [ <expr> | ( "back" <expr> ) | "home" ]
      case 'w':
        return RWalk(err);
        continue;
      default:
        throw NotImpli(st.peek());
    }
  }while(true); //*/
}

//"walk " [ <Int> | ( "back" <Int> ) | "home" ]
Token *Lex::RWalk(LexErr &err){ 
  #ifdef DEBUG
  std::cout << "Calling RWalk" << std::endl;
  #endif

  int errCnt = 0;
  ConsumTerm<std::isalnum>('w', err, errCnt, 40);
  ConsumTerm<std::isalnum>('a', err, errCnt, 25);
  ConsumTerm<std::isalnum>('l', err, errCnt, 13);
  ConsumTerm<std::isalnum>('k', err, errCnt, 12);
  TryConsumTerm(' ', err);

  switch (err.state){
    case LexErr::Status::OK:
      if (errCnt < 75) return new Token(new char[] { 'w','a','l','k'},TokE::Walk);
    case LexErr::Status::WARN:
    case LexErr::Status::ERROR:
      err.ResetError();
      err.AddWalkLexingError();
      return nullptr;
    case LexErr::Status::FATAL:
      return nullptr;
  }
}

Token *Lex::RInt(){
  //int ret;
  //bool neg = false;
  if(st.peek() == '-'){
    sb += st.get();
  }
  if(!std::isdigit(st.peek()))
    return nullptr; //error

  do{
    sb += st.get();
  } while(std::isdigit(st.peek()));
  return new Token(sb.c_str(),TokE::Int);
}

LexErr &Lex::ConsumTerm(char a, LexErr &err){
  if(st.peek() == a){
    READ;
    return err;
  }
  READ;
  //error
  return err.ConsumedWrong(a);
}

LexErr &Lex::TryConsumTerm(char a, LexErr &err){
  if(st.peek() == a){
    READ;
    return err;
  }
  //error
  return err.TryConsumedWrong(a);

}

#ifdef U_TEST
void Lex::swapStream(std::ifstream &a){
  st.close();
  st.swap(a);
}
void Lex::swapStream(const char *a){
  st.close();
  new(&st) std::ifstream(a);
}

#endif
