#pragma once

class CodeGenerator;

#include "libs/SceneParser.h"
#include "src/Variable.h"

#include <ParserRuleContext.h>
#include <gtest/gtest_prod.h>
#include <string>
#include <unordered_map>
#include <vector>


class Function{
  protected:
    const VarType retType;
    const std::string name;
    std::vector<std::pair<std::string, Variable>> vars;
    antlr4::ParserRuleContext *ctx;
  public:
    Function(const std::string nm, VarType retT, SceneParser::PathdefContext *CTX);
    Function(const std::string nm, VarType retT, SceneParser::CalcdefContext *CTX);
    Function(const std::string nm, VarType retT, SceneParser::MainContext *CTX);
    std::string getFunctionDeclaration();
    std::string getFunctionCall(const std::vector<Variable> var);
    std::string Implement();
    std::string getName() { return  name; }
    const std::vector<std::pair<std::string, Variable>> getHeaderVars() { return vars; }
    FRIEND_TEST(FunctionHandler, CTor);
};
