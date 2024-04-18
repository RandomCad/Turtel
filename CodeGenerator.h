#ifndef MY_VISITOR_FILE
#define MY_VISITOR_FILE

#include "libs/SceneBaseVisitor.h"

class CodeGenerator : public SceneBaseVisitor{
  
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
};

#endif
