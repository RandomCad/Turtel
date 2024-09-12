#pragma once

#include "SceneBaseVisitor.h"
#include "src/VariableHeandler.h"
class MathVisitor : public SceneBaseVisitor{
  private:
    VariableHeandler _variables;

  public:
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

  private:
#ifdef UNIT_TEST
    friend bool TestNumberExec(TestError*&ret);
    friend bool TestExprExec(TestError *&ret);
    friend bool TestCodeGeneratorProgramBase(TestError *&);
    friend bool TestCodeGeneratorEndMain(TestError *&);
    friend bool TestCodeGeneratorDTOr(TestError *&);
    friend bool TestCodeGeneratorEmpty(TestError *&);
    friend bool TestCodeGeneratorCTor(TestError *&);
#endif
};
