#pragma once
#include <string>

enum VarType{
  RENDERER,
  DOUBLE,
};

struct Variable{
  VarType type;
  std::string name;
  bool isUnique;
  Variable(VarType Type, std::string nm, bool is = false) : type(Type), name(nm), isUnique(is){}
  Variable(){}
  std::string &getName(){ return name;}
  const std::string &getName() const{ return name;}
  std::string getTypeAndName() const;
};
