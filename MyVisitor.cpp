#include "MyVisitor.h"
#include "SceneParser.h"
#include <any>
#include <string>

std::any MyVisitor::visitInt(SceneParser::IntContext *ctx){
  int ret = std::stoi(ctx->Num()->getSymbol()->getText());
  return ret;
}

std::any MyVisitor::visitFloat(SceneParser::FloatContext *ctx){
  double ret = std::stod(ctx->Float()->getSymbol()->getText());
  return ret;
}

std::any MyVisitor::visitExp(SceneParser::ExpContext *ctx){
}
