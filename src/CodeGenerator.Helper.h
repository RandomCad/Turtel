#pragma once

#include "src/VariableHeandler.h"
#include <ostream>

template<typename T>
void CalcPosX(
    const T len,
    std::ostream &output,
    const char *xName, 
    const char *rotName){
  output << xName << " + " << len << " * cos(" << rotName << ')';
}
template<typename T>
void CalcPosX(
    const T len,
    std::ostream &output,
    VariableHeandler &vars){
  output  << vars.getVariableNameString("@x") 
          << " + " 
          << len 
          << " * "
          << "cos(" << vars.getVariableNameString("@rot") << ')';
}

template<typename T>
void CalcPosY(
    const T len,
    std::ostream &output,
    const char *yName, 
    const char *rotName){
  output << yName << " + " << len << " * sin(" << rotName << ')';
}
template<typename T>
void CalcPosY(
    const T len,
    std::ostream &output,
    VariableHeandler &vars){
  output  << vars.getVariableNameString("@y") 
          << " + " 
          << len 
          << " * "
          << "sin(" << vars.getVariableNameString("@rot") << ')';
}
