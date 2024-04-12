#ifndef MY_VISITOR_FILE
#define MY_VISITOR_FILE

#include "libs/SceneBaseVisitor.h"

class MyVisitor : public SceneBaseVisitor{
  std::any visitInt(SceneParser::IntContext *ctx) override;
  std::any visitFloat(SceneParser::FloatContext *ctx) override;
  std::any visitExp(SceneParser::ExpContext *ctx) override;
  std::any visitNumExpr(SceneParser::NumExprContext *ctx) override;
};

#endif
