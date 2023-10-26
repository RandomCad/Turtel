#ifndef TOKEN
#define TOKEN
#include <string.h>

enum TokE{
  Walk,
  Int
};

struct Token{
  Token *next;
  Token *prev;
  const char *str;
  const int tok;

  Token(Token *nex, Token *pre,const char *sb,const int to) : next(nex), prev(pre), 
        str(strdup(sb)), tok(to) {}
  Token(Token *nex,const char *st,const int to) : next(nex), str(strdup(st)), tok(to) {}
  Token(const char *st,const  int to) : str(strdup(st)), tok(to) {}

  void AddNext(Token *next){
    this->next = next;
  }

  void AddPrev(Token *prev){
    this->prev = prev;
  }
  
};

#endif
