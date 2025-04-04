#include "src/Function.h"
#include "SceneParser.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/VariableVisitor.h"
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>

/**
 * @brief Constructs a Function object for a PathdefContext.
 * 
 * @param nm The name of the function.
 * @param retT The return type of the function. Should be void
 * @param CTX The PathdefContext of the function.
 */
Function::Function(const std::string nm, VarType retT, SceneParser::PathdefContext *CTX) 
      : retType(retT), name(nm), ctx(CTX) {
  if(!CTX->paramlist()){
    return;
  }
  vars.reserve(CTX->paramlist()->var().size());
  for (auto i : CTX->paramlist()->var()) {
    std::string nm = i->children[0]->getText();
    Variable nw(VarType::DOUBLE, "__usr_func_" + nm);
    nw.isDefined = true;
    vars.push_back({nm,nw});
  }
}

/**
 * @brief Constructs a Function object with a name, return type, and parsing context.
 *
 * Initializes the function's name, return type, and context. If the context includes a parameter list,
 * it creates corresponding Variable objects of type DOUBLE, marks them as defined, and adds them to the vars vector.
 *
 * @param nm The function's name.
 * @param retT The function's return type.
 * @param CTX Pointer to the CalcdefContext containing the function's parsing context.
 */
Function::Function(const std::string nm, VarType retT, SceneParser::CalcdefContext *CTX) 
      : retType(retT), name(nm), ctx(CTX) {
  if(!CTX->paramlist()){
    return;
  }
  vars.reserve(CTX->paramlist()->var().size());
  for (auto i : CTX->paramlist()->var()) {
    std::string nm = i->children[0]->getText();
    Variable nw(VarType::DOUBLE, "__usr_func_" + nm);
    nw.isDefined = true;
    vars.push_back({nm,nw});
  }
}

/**
 * @brief Constructs a Function object for a MainContext.
 * 
 * @param nm The name of the function.
 * @param retT The return type of the function.
 * @param CTX The MainContext of the function.
 */
Function::Function(const std::string nm, VarType retT, SceneParser::MainContext *CTX) 
      : retType(retT), name(nm), ctx(CTX) {
}

/**
 * @brief Returns the function declaration for this function.
 * 
 * This function generates and returns the function declaration as a string.
 * 
 * @return The function declaration as a string.
 */
std::string Function::getFunctionDeclaration(){
  //first add the returntype
  std::string ret = VarTypeNS::getTypeName(this->retType);
  //no the name and the opening '('
  ret += name + "(";
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

/**
 * @brief Returns the function call for this function.
 * 
 * This function generates and returns the function call as a string, given a vector of variables.
 * 
 * @param var The vector of variables to use in the function call.
 * @return The function call as a string.
 */
std::string Function::getFunctionCall(const std::vector<Variable> var){
#ifndef NDEBUG
std::cerr << __func__ << std::endl;
#endif  
  if(var.size() != vars.size()){
    std::cerr << "input length unequal to function length" << vars.size() << ' ' << var.size() << std::endl;
    throw "input length unequal function length";
  }
  //no the name and the opening '('
  std::string ret = name + '(';
  //add all the variables
  for (auto i : var) {
    ret += i.getName() + ", ";
  }
  if(vars.size() > 0){
    //add the ')' and the ';'
    ret[ret.size()-2] = ')';
    ret[ret.size()-1] = ' ';
  }
  else {
    ret += ')';
  }
  return ret;
}

/**
 * @brief Generates the function signature for implementation.
 *
 * Constructs a string representing the function's signature, including its
 * return type, name, and parameters, formatted appropriately for the
 * function's implementation.
 *
 * @return A string containing the formatted function signature.
 */
std::string Function::Implement(){
  //prepere the variable part of the string
  std::string ret = VarTypeNS::getTypeName(retType);
  ret += name;
  ret += "(";
  for (auto i : vars) {
    ret += i.second.getTypeAndName() + ", ";
  }
  if(vars.size() > 0){
    ret[ret.size() - 2] = ')';
    ret[ret.size() - 1] = '{';
  }
  else ret += "){";
  return ret;
}
