
#include "SceneVisitor.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/FunctionHandler.h"
#include "src/VariableVisitor.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

FuctionHandler::FuctionHandler(SceneParser::MainContext* mainCtx, 
                    std::vector<SceneParser::PathdefContext*> paths,
                    std::vector<SceneParser::CalcdefContext*> calcs){
  //funcs.emplace("main", MainFunction(mainCtx));
  for (auto i : paths) {
    funcs.emplace ( i->ID()->getText(), 
        Function(i->ID()->getText()
          , VarVisitor().getVariableContext(i->paramlist())
          , VarType::VOID
          , i)
        );
  }
  for (auto i : calcs) {
    funcs.emplace ( i->ID()->getText(), 
        Function(i->ID()->getText()
          , VarVisitor().getVariableContext(i->paramlist())
          , VarType::DOUBLE
          , i)
        );
  }
}

std::string FuctionHandler::getFunctionDeclaration(std::string funcName){
  return funcs.at(funcName).getFunctionDeclaration();
}

std::string FuctionHandler::getFunctionCall(std::string funcName, std::vector<Variable> input){
#ifndef NDEBUG
std::cerr << __func__ << std::endl;
#endif  
  return funcs.at(funcName).getFunctionCall(input);
}

void FuctionHandler::ImplementFunctions(SceneVisitor *vis){
  for (auto i : funcs) {
    std::cerr << &i << std::endl;
    i.second.Implement(vis);
    std::cerr << &i << "ende" << std::endl;
  }
  std::cerr << "exiting" << __func__ << std::endl;
}

void FuctionHandler::getFunctionDeclarations(std::ostream &out){
  /*
  for (auto i : funcs) {
    out << i.second.getFunctionDectaration();
  }*/
}
