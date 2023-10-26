#include "Token.h"
#include "Lexer.h"
#include <cctype>
#include <fstream>

Token *Lex::GetToken(){
  if(!*this){
    //error
    return nullptr;

  }
  do{
    switch (peek()){
      case ' ':
      case '\t':
      case '\n':
      case '\v':
      case '\f':
      case '\r':
        get();
        continue;
          
      //"walk " [ <expr> | ( "back" <expr> ) | "home" ]
      case 'w':
        get();
        RWalk();
        continue;
    }
  }while(true);
}

//"walk " [ <Int> | ( "back" <Int> ) | "home" ]
Token *Lex::RWalk(){ 
  ConsumTerm('a');
  ConsumTerm('l');
  ConsumTerm('k');
  ConsumTerm(' ');
  switch (peek()){
    case 'b':
      
      //role
    case 'h':
      //role
    default:
      Token *zwi = new Token(RInt(),new char[] { 'w','a','l','k'},TokE::Walk);
      zwi->next->prev = zwi;
      return zwi;

      //expr role
      break;
  }
}

Token *Lex::RInt(){
  int ret;
  bool neg = false;
  if(peek() == '-'){
    sb += get();
  }
  if(!std::isdigit(peek()))
    return nullptr; //error

  do{
    sb += get();
  } while(std::isdigit(peek()));
  return new Token(sb.c_str(),TokE::Int);
}

bool Lex::ConsumTerm(char a){
  if(peek() == a){
    get();
    return true;
  }
  //error
  return false;
}
