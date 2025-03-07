#include "libs/SceneParser.h"
#include "libs/SceneVisitor.h"
#include "src/Variable.h"

#include <ParserRuleContext.h>
#include <gtest/gtest_prod.h>
#include <string>
#include <unordered_map>
#include <vector>

#pragma once

class Function{
  protected:
    const VarType retType;
    const std::string name;
    std::vector<Variable> vars;
    std::unordered_map<std::string, Variable> varCtx;
    antlr4::ParserRuleContext *ctx;
  public:
    Function(const std::string nm, VarType retT, SceneParser::PathdefContext *CTX);
    Function(const std::string nm, VarType retT, SceneParser::CalcdefContext *CTX);
    Function(const std::string nm, VarType retT, SceneParser::MainContext *CTX);
    std::string getFunctionDeclaration();
    std::string getFunctionCall(const std::vector<Variable> var);
    void Implement(std::ostream &out, SceneVisitor *vis);

    FRIEND_TEST(FunctionHandler, CTor);
};
