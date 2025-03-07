#ifndef MY_VISITOR_FILE
#define MY_VISITOR_FILE

#include <ParserRuleContext.h>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <vector>

#include "../libs/SceneParser.h"
#include "../libs/SceneBaseVisitor.h"
#include "build/_deps/googletest-src/googletest/include/gtest/gtest_prod.h"
#include "src/TopLevelVisitor.h"
#include "src/VariableHeandler.h"
#include "src/FunctionHandler.h"
#include "../UnitTest/TestSuits.h"

#define CALC_POS_X(len) POSITION_X_NAME + len * cos(ROTATION_NAME)
#define CALC_POS_Y(len) POSITION_Y_NAME + len * sin(ROTATION_NAME)


size_t GetUniquNumber();

class CodeGenerator : public SceneBaseVisitor{
  private:
    std::ostream &output;
    SceneParser::FileContext *astBase;
    SceneParser::MainContext *astMain;
    std::vector<SceneParser::CalcdefContext *> astCalcdef;
    std::vector<SceneParser::PathdefContext *> astPathdef;

    VariableHeandler _variables;
    FuctionHandler _funcs;
    TopLevelVisitor _topVis;
    CodeGenerator(std::ostream &outStream);
  public:
    CodeGenerator(std::ostream &outStream, SceneParser::FileContext *AstBase);

    ///Main function of the classe
    void GenerateCode();
    
  private:
    std::any visitMain(SceneParser::MainContext *ctx) override;
    void AddIncludes();
    void AddFunctionDeclaration();
    void AddGlobalVars();
    void AddMain();
    void AddTurtelFunctions();

    void ProgrammBase();
    void EndeMain();
    void ImplementFunction(std::vector<Variable>&, antlr4::ParserRuleContext *);

    FRIEND_TEST(CodeGenerator, AddMain);
    FRIEND_TEST(CodeGenerator, BasicEmptyMain);
    FRIEND_TEST(CodeGenerator, BasicWalk);
    FRIEND_TEST(CodeGenerator, AddFunctionDeclaration);

};

#endif
