#pragma once
#include <string>

///Defince all posible types of varaiables for the transpiler 
enum VarType{
  RENDERER, ///< The type of the SDL2 renderer in \ref VariableHeandler::enviornment
  DOUBLE, ///< The most comon type a normal c-double
  CONST_DOUBLE, ///< Constant c-double
  TESXTUR, ///< The sdl texture type
  //SURFACE, ///< The sdl surface for output of immages
};

struct Variable{
  VarType type;
  std::string name;
  bool isUnique;
  Variable(VarType Type, std::string nm, bool is = false) : type(Type), name(nm), isUnique(is){}
  Variable(){}
  const std::string &getName() const { return name;}
  std::string getTypeAndName() const;
};
