#ifndef MY_VISITOR_FILE
#define MY_VISITOR_FILE

#include <iostream>
#include <iterator>
#include <ostream>

#include "libs/SceneBaseVisitor.h"
#include "UnitTest.h"

class CodeGenerator : public SceneBaseVisitor{
  private:
    std::ostream &output;
    //Only used for Unittesting
    CodeGenerator(): output(std::cout) {}
  public:
    CodeGenerator(std::ostream &outStream);
  
    //Number
    std::any visitInt(SceneParser::IntContext *ctx) override;
    std::any visitFloat(SceneParser::FloatContext *ctx) override;
    //Expr
    std::any visitNumExpr(SceneParser::NumExprContext *ctx) override;
    std::any visitExp(SceneParser::ExpContext *ctx) override;
    std::any visitAdd(SceneParser::AddContext *ctx) override;
    std::any visitDim(SceneParser::DimContext *ctx) override;
    std::any visitDife(SceneParser::DifeContext *ctx) override;
    std::any visitMult(SceneParser::MultContext *ctx) override;
    std::any visitABS(SceneParser::ABSContext *ctx) override;
    std::any visitNegate(SceneParser::NegateContext *ctx) override;
    std::any visitWalk(SceneParser::WalkContext *ctx) override;
  
  private:
    void ProgrammBase();
    void EndeMain();

    friend bool TestNumberExec(TestError*&ret);
    friend bool TestExprExec(TestError *&ret);
};

#endif
