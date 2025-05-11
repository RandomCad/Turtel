#pragma once
#include "build/_deps/googletest-src/googletest/include/gtest/gtest_prod.h"
#include <string>

class Function;

///Defince all possible types of variables for the transpiler 
enum VarType{
  RENDERER, ///< The type of the SDL2 renderer in \ref VariableHeandler::enviornment
  DOUBLE, ///< The most common type a normal c-double
  CONST_DOUBLE, ///< Constant c-double
  TESXTUR, ///< The sdl texture type
  //SURFACE, ///< The sdl surface for output of immages
  WINDOW, ///< The main and only window of the program
  CONST_CHAR_PTR,
  VOID,
  EVENT,
  SIZE_T,
  DOUBLE_ARR,
};

namespace VarTypeNS{
  std::string getTypeName(VarType type);
}

struct Variable{
  VarType type;
  std::string name;
  bool isDefined = false;
  Variable(VarType Type, std::string nm) : type(Type), name(nm){}
  Variable(){}
  const std::string &getName() const { return name;}
  private:
  std::string getTypeAndName() const;
  public:
  ///@returns a code line that implements this variable and assigns it assigne
  const std::string getDefinition(const std::string assigne = "0");
  FRIEND_TEST(Variable, TestGetNameAndType);
  friend class Function;
};
