#pragma once
#include <string>

enum VarType{
  RENDERER
};

struct Variable{
  VarType type;
  std::string name;
  bool isUnique;
  Variable(VarType Type, std::string nm, bool is = false) : type(Type), name(nm), isUnique(is){}
  Variable(){}
  std::string &getName(){ return name;}
  std::string getTypeAndName();
};
