#include "MyVisitor.h"
#include "SceneParser.h"
#include <any>
#include <stdexcept>
#include <string>

std::any MyVisitor::visitInt(SceneParser::IntContext *ctx){
  int ret = std::stoi(ctx->Num()->getSymbol()->getText());
  return ret;
}

std::any MyVisitor::visitFloat(SceneParser::FloatContext *ctx){
  double ret = std::stod(ctx->Float()->getSymbol()->getText());
  return ret;
}

std::any MyVisitor::visitNumExpr(SceneParser::NumExprContext *ctx){
  std::any number = ctx->number()->accept(this);
  if (number.type() == typeid(int)){
    return std::to_string(std::any_cast<int>(number));
  }
  else if (number.type() == typeid(double)){
    return std::to_string(std::any_cast<double>(number));
  }
  else{
    throw std::runtime_error("coudn't cast number context to number");
  }
}

std::any MyVisitor::visitExp(SceneParser::ExpContext *ctx){
  std::any child1 = ctx->children[0]->accept(this);
  std::any child2 = ctx->children[2]->accept(this);
}
