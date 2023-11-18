#include "Token.h"
#include "Lexer.h"
#include "LexerError.h"

#include <cctype>
#include <fstream>

std::ostream &operator<< (std::ostream &a, Token &b){
  return a << "List pointer (prev,next): " << b.prev << ',' << b.next << "\nToken str: " << b.str << "\nToken type: " << b.tok << std::endl;
}

Token *Lex::GetToken(){
  #ifdef DEBUG
  std::cout << "Calling GetToken" << std::endl;
  #endif
  do{
    if(!st){
      #ifdef DEBUG
      std::cout << "EOF" << std::endl;
      #endif
      //error
      throw EOFErr();
    }
    else switch (st.peek()){
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
        return RWalk();
        continue;
      default:
        throw NotImpli();
    }
  }while(true);
}

//"walk " [ <Int> | ( "back" <Int> ) | "home" ]
Token *Lex::RWalk(){ 
  #ifdef DEBUG
  std::cout << "Calling RWalk" << std::endl;
  #endif

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
