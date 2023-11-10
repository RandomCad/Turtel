#ifndef LEXER_ERROR
#define LEXER_ERROR

#include <exception>

class LexErr: public std::exception{
};

class EOFErr : public LexErr{
  public:
    const char * what(){
      return "Encounterd end of file, whene this wasn't expected";
    }
};

//TODO save what is wrong
class NotImpli : public LexErr{
  public:
    const char * what(){
      return "Encounterd symbole for which no lexing path was founde"; 
    }
};

#endif
