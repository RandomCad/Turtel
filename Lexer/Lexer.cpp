#include "Token.h"
#include "Lexer.h"
#include <cctype>
#include <fstream>

std::ostream &operator<< (std::ostream &a, Token &b){
  return a << "List pointer (prev,next): " << b.prev << b.next << "\nToken str: " << b.str << "\nToken type: " << b.tok << std::endl;
}

Token *Lex::GetToken(){
  if(!st){
    //error
    return nullptr;

  }
  do{
    switch (st.peek()){
      case ' ':
      case '\t':
      case '\n':
      case '\v':
      case '\f':
      case '\r':
        st.get();
        continue;
          
      //"walk " [ <expr> | ( "back" <expr> ) | "home" ]
      case 'w':
        st.get();
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
  switch (st.peek()){
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

bool Lex::ConsumTerm(char a){
  if(st.peek() == a){
    st.get();
    return true;
  }
  //error
  return false;
}
