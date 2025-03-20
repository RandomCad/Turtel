
#include "SceneParser.h"
#include "src/CodeGenerator.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/FunctionHandler.h"
#include "src/Function.h"
#include "src/VariableVisitor.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::vector<Variable> getVars(SceneParser::ParamlistContext *ctx){
  if (!ctx){
    return {};
  }
  std::vector<Variable> ret;
  ret.reserve(ctx->var().size());

  std::transform(
    ctx->var().begin(), 
    ctx->var().end(), 
    std::back_inserter(ret),
    [](const SceneParser::VarContext *s) -> Variable { 
      return Variable(VarType::DOUBLE, "__usr_func_" + s->children[0]->getText());
    }
  );
  return ret;
}

FunctionHandler::FunctionHandler(SceneParser::MainContext* mainCtx, 
                    std::vector<SceneParser::PathdefContext*> &paths,
                    std::vector<SceneParser::CalcdefContext*> &calcs){
  funcs.emplace(
      MAIN_FUNC, 
      Function(TURTEL_MAIN_FUNC_NAME, VarType::VOID, mainCtx)
      );
  for (auto i : paths) {
    if(!funcs .emplace( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::VOID,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
  for (auto i : calcs) {
    if(!funcs .emplace ( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::DOUBLE,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
}
FunctionHandler::FunctionHandler(SceneParser::MainContext* mainCtx, 
                    std::vector<SceneParser::PathdefContext*> &&paths,
                    std::vector<SceneParser::CalcdefContext*> &&calcs){
  funcs.emplace(
      MAIN_FUNC, 
      Function(TURTEL_MAIN_FUNC_NAME, VarType::VOID, mainCtx)
      );
  for (auto i : paths) {
    if(!funcs .emplace( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::VOID,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
  for (auto i : calcs) {
    if(!funcs .emplace ( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::DOUBLE,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
}

std::string FunctionHandler::getFunctionDeclaration(std::string funcName){
  return funcs.at(funcName).getFunctionDeclaration();
}

std::string FunctionHandler::getFunctionCall(std::string funcName, std::vector<Variable> input){
#ifndef NDEBUG
std::cerr << __func__ << std::endl;
#endif  
  return funcs.at(funcName).getFunctionCall(input);
}

void FunctionHandler::ImplementFunctions(std::ostream &out, CodeGenerator *vis){
  for (auto i : funcs) {
    std::cerr << &i << std::endl;
    i.second.Implement(out, vis);
    std::cerr << &i << "ende" << std::endl;
  }
  std::cerr << "exiting" << __func__ << std::endl;
}

void FunctionHandler::getFunctionDeclarations(std::ostream &out){
  for (auto i : funcs) {
    out << i.second.getFunctionDeclaration();
  }
}
