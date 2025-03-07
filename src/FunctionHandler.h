#include "SceneParser.h"
#include "SceneVisitor.h"
#include "src/Function.h"
#include "src/Variable.h"
#include <gtest/gtest_prod.h>
#include <string>
#include <vector>
#include <unordered_map>
#pragma once

class FuctionHandler{
  std::unordered_map<std::string, Function> funcs;
  std::string getFunctionDeclaration(std::string funcName);
  public:
    FuctionHandler(SceneParser::MainContext*, 
                    std::vector<SceneParser::PathdefContext*>&&,
                    std::vector<SceneParser::CalcdefContext*>&&);
    FuctionHandler(SceneParser::MainContext*, 
                    std::vector<SceneParser::PathdefContext*>&,
                    std::vector<SceneParser::CalcdefContext*>&);
    std::string getFunctionCall(std::string funcName, std::vector<Variable> input);
    void ImplementFunctions(std::ostream &, SceneVisitor *vis);
    void getFunctionDeclarations(std::ostream &out);

    FRIEND_TEST(FunctionHandler, CTor);
};
