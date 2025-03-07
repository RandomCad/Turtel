#include "src/Function.h"
#include "SceneParser.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/VariableVisitor.h"
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>

Function::Function(const std::string nm, VarType retT, SceneParser::PathdefContext *CTX) 
      : retType(retT), name(nm), ctx(CTX) {
  if(!CTX->paramlist()){
    varCtx = VarVisitor().getVariableContext(CTX->statList());
    return;
  }
  vars.reserve(CTX->paramlist()->var().size());
  for (auto i : CTX->paramlist()->var()) {
    std::string nm = i->children[0]->getText();
    Variable nw(VarType::DOUBLE, "__usr_func_" + nm);
    vars.push_back(nw);
    varCtx.insert({nm, nw});
  }
  varCtx.merge(VarVisitor().getVariableContext(CTX->statList()));
}
Function::Function(const std::string nm, VarType retT, SceneParser::CalcdefContext *CTX) 
      : retType(retT), name(nm), ctx(CTX) {
  if(!CTX->paramlist()){
    varCtx = VarVisitor().getVariableContext(CTX->statList());
    return;
  }
  vars.reserve(CTX->paramlist()->var().size());
  for (auto i : CTX->paramlist()->var()) {
    std::string nm = i->children[0]->getText();
    Variable nw(VarType::DOUBLE, "__usr_func_" + nm);
    vars.push_back(nw);
    varCtx.insert({nm, nw});
  }
  varCtx.merge(VarVisitor().getVariableContext(CTX->statList()));
}
Function::Function(const std::string nm, VarType retT, SceneParser::MainContext *CTX) 
      : retType(retT), name(nm), ctx(CTX) {
  vars.reserve(1);
  vars.push_back(Variable(VarType::RENDERER,"__env_rnd")); 
  varCtx.insert({RND_NAME, Variable(VarType::RENDERER,"__env_rnd")}); 
  
  varCtx.merge(VarVisitor().getVariableContext(CTX->statList()));
}
///returns the function declaration for this function
std::string Function::getFunctionDeclaration(){
  //first add the returntype
  std::string ret = VarTypeNS::getTypeName(this->retType);
  //no the name and the opening '('
  ret += name + "(";
  //add all the variables
  for (auto i : vars) {
    ret += i.getTypeAndName() + ", ";
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
  std::string ret = " __fun_" + name + "(";
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

void Function::Implement(std::ostream &out, SceneVisitor *vis){
  //prepere the variable part of the string
  std::string ret = "(";
  for (auto i : vars) {
    ret += i.getTypeAndName() + ", ";
  }
  if(vars.size() > 0){
    ret[ret.size() - 2] = ')';
    ret[ret.size() - 1] = '{';
  }
  else ret += "){";
  out << VarTypeNS::getTypeName(retType)
      << " __func_"
      << name
      << ret
      << '\n'
      ;
  ctx->accept(vis);
  out << "}\n";
}
