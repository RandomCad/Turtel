#include "LexerError.h"
#include "../Helper.h"


LexErr &LexErr::TryConsumedWrong(char corr){
  Error::S zwi;
  zwi.car = corr;
  error.push(Error(Error::errorNum::TryWrongConsum,zwi, sb.length() - 1));
  return *this;
}
       
void LexErr::AddWalkLexingError(){
  Error::S zwi = {nullptr};
  error.push(Error(Error::errorNum::WalkLexErr,zwi, sb.length() - 1));
}

LexErr &LexErr::ConsumedWrong(char corr){
  Error::S zwi;
  zwi.car = corr;
  error.push(Error(Error::errorNum::WrongConsum,zwi, sb.length() - 1));
  state = (state == Status::OK) ? Status::WARN : state;
  return *this;
}
LexErr &LexErr::ConsumedWrong(){
  Error::S zwi = {nullptr};
  error.push(Error(Error::errorNum::WrongConsum,zwi, sb.length() - 1));
  state = (state == Status::OK) ? Status::WARN : state;
  return *this;
}

std::ostream &LexErr::PrintError(std::ostream &a, LexErr::Error &err){
  std::cerr << "test" << std::endl;
  switch(err.err){
    case LexErr::Error::errorNum::EndOfFile:
    case LexErr::Error::errorNum::WrongConsum:
    case LexErr::Error::errorNum::TryWrongConsum:
    case LexErr::Error::errorNum::WalkLexErr:
      a << "REDNotImplimentedCLEAR";
      break;
  }
  return a;
}

std::ostream &operator<< ( std::ostream &a, LexErr &b){
  while (!b.error.empty()){
  std::cerr << "test" << std::endl;
    LexErr::PrintError(a,b.error.front());
    b.error.pop();
    
  }
  return a;
}
