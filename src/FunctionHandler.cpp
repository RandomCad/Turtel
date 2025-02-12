
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/FunctionHandler.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
std::unordered_map<std::string, std::shared_ptr<Function>> FuctionHandler::funcs = {
  {
    END_FUNC,
    std::shared_ptr<Function>(
      new HelperFunc(
        END_FUNC_NAME,
        std::vector<Variable>{
          Variable(VarType::DOUBLE, "retCode"),
          Variable(VarType::RENDERER, "__env_rnd"),
        },
        VarType::VOID
      )
    )
  },
  {
    IMMAGE_FUNC,
    std::shared_ptr<Function>(
      new HelperFunc(
        IMMAGE_FUNC_NAME,
        std::vector<Variable>{
          Variable(VarType::CONST_CHAR_PTR, "file_name"),
          Variable(VarType::RENDERER, "renderer"),
          Variable(VarType::TESXTUR, "texture")
        },
        VarType::VOID
      )
    )
  }
};

std::string FuctionHandler::getFunctionDeclaration(std::string funcName){
  return funcs.at(funcName)->getFunctionDectaration();
}

std::string FuctionHandler::getFunctionCall(std::string funcName, std::vector<Variable> input){
#ifndef NDEBUG
std::cerr << __func__ << std::endl;
#endif  
  return funcs.at(funcName)->getFunctionCall(input);
}

void FuctionHandler::ImplementFunctions(std::ostream &out){
  for (auto i : funcs) {
    std::cerr << &i << std::endl;
    i.second->Implement(out);
    std::cerr << &i << "ende" << std::endl;
  }
  std::cerr << "exiting" << __func__ << std::endl;
}

void FuctionHandler::getFunctionDeclarations(std::ostream &out){
  for (auto i : funcs) {
    out << i.second->getFunctionDectaration();
  }
}
