#pragma once

class CodeGenerator;
class Function;
class TopLevelVisitor;

#include "../libs/SceneParser.h"
#include "src/Variable.h"
#include "src/Function.h"
#include <gtest/gtest_prod.h>
#include <string>
#include <vector>
#include <unordered_map>

class FunctionHandler{
  std::unordered_map<std::string, Function> funcs;
  std::string getFunctionDeclaration(std::string funcName);
  public:
    FunctionHandler(SceneParser::MainContext*, 
                    std::vector<SceneParser::PathdefContext*>&&,
                    std::vector<SceneParser::CalcdefContext*>&&);
    FunctionHandler(SceneParser::MainContext*, 
                    std::vector<SceneParser::PathdefContext*>&,
                    std::vector<SceneParser::CalcdefContext*>&);
    std::string getFunctionCall(std::string funcName, std::vector<Variable> input);
    void ImplementFunctions(std::ostream &, CodeGenerator *vis);
    void getFunctionDeclarations(std::ostream &out);
    bool Conatains(const std::string &name){
      return funcs.count(name);
    }
    FunctionHandler(){};
  private:
    friend class TopLevelVisitor;
    FRIEND_TEST(FunctionHandler, CTor);
};
