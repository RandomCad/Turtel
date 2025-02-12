#include "src/Function.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/VariableHeandler.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>

///returns the function declaration for this function
std::string Function::getFunctionDeclaration(){
  //first add the returntype
  std::string ret = VarTypeNS::getTypeName(this->retType);
  //no the name and the opening '('
  ret += " __fun_" +  name + "(";
  //add all the variables
  for (auto i : vars) {
    ret += i.second.getTypeAndName() + ", ";
  }
  if(vars.size() > 0){
    //add the ')' and the ';'
    ret[ret.size()-2] = ')';
    ret[ret.size()-1] = ';';
  }
  else {
    ret += ");";
  }
  //add a '\n' and return the result
  return ret + '\n';
}

std::string Function::getFunctionCall(const std::vector<Variable> var){
#ifndef NDEBUG
std::cerr << __func__ << std::endl;
#endif  
  if(var.size() != vars.size()){
    std::cerr << "input length unequal to function length" << std::endl;
    throw "input length unequal function length";
  }
  //no the name and the opening '('
  std::string ret = name + "(";
  //add all the variables
  for (auto i : var) {
    ret += i.getName() + ", ";
  }
  if(vars.size() > 0){
    //add the ')' and the ';'
    ret[ret.size()-2] = ')';
    ret[ret.size()-1] = ';';
  }
  else {
    ret += ");";
  }
  //add a '\n' and return the result
  return ret;
}

void Function::Implement(SceneVisitor *vis){
  ctx->accept(vis);
}
