#include "MyVisitor.h"
#include "SceneParser.h"
#include <any>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>

std::any MyVisitor::visitInt(SceneParser::IntContext *ctx){
  int64_t ret = std::stoi(ctx->Num()->getSymbol()->getText());
  return ret;
}

std::any MyVisitor::visitFloat(SceneParser::FloatContext *ctx){
  double ret = std::stod(ctx->Float()->getSymbol()->getText());
  return ret;
}

std::any MyVisitor::visitABS(SceneParser::ABSContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  if (ret.type() == typeid(int64_t))
    return std::abs(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))
    return std::abs(std::any_cast<double>(ret));
  else if(ret.type() == typeid(std::string))
    return "fabs(" + std::any_cast<std::string>(ret) + ')';
  else
    throw std::runtime_error("todo:"); //TODO:
}

std::any MyVisitor::visitNegate(SceneParser::NegateContext *ctx){
  std::any number = ctx->number()->accept(this);
  if (number.type() == typeid(int64_t)){
    return -std::any_cast<int64_t>(number);
  }
  else if (number.type() == typeid(double)){
    return -std::any_cast<double>(number);
  }
  else{
    throw std::runtime_error("coudn't cast number context to number");
  }
}

std::any MyVisitor::visitNumExpr(SceneParser::NumExprContext *ctx){
  std::any number = ctx->number()->accept(this);
  if (number.type() == typeid(int64_t)){
    return std::any_cast<int64_t>(number);
  }
  else if (number.type() == typeid(double)){
    return std::any_cast<double>(number);
  }
  else{
    throw std::runtime_error("coudn't cast number context to number");
  }
}

std::any MyVisitor::visitAdd(SceneParser::AddContext *ctx){
  std::any left = ctx->children[0]->accept(this);
  std::any reigth = ctx->children[2]->accept(this);

  if(left.type() == typeid(std::string) && reigth.type() == typeid(std::string))
    return std::any_cast<std::string>(left) + '+' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(int64_t))
    return std::any_cast<std::string>(left) + '+' + std::to_string(std::any_cast<int64_t>(reigth));
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(double))
    return std::any_cast<std::string>(left) + '+' + std::to_string(std::any_cast<double>(reigth));
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<int64_t>(left)) + '+' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<double>(left)) + '+' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(int64_t))
    return std::any_cast<int64_t>(left) + std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(int64_t))
    return std::any_cast<double>(left) + std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(double))
    return std::any_cast<int64_t>(left) + std::any_cast<double>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(double))
    return std::any_cast<double>(left) + std::any_cast<double>(reigth);
  else{
    throw std::runtime_error("todo:"); //TODO:
  }
}

std::any MyVisitor::visitExp(SceneParser::ExpContext *ctx){
  std::any left = ctx->children[0]->accept(this);
  std::any reigth = ctx->children[2]->accept(this);

  if(left.type() == typeid(std::string) && reigth.type() == typeid(std::string))
    return "pow(" + std::any_cast<std::string>(left) + ',' + std::any_cast<std::string>(reigth) + ')';
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(int64_t))
    return std::any_cast<std::string>(left) + '+' + std::to_string(std::any_cast<int64_t>(reigth)) + ')';
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(double))
    return std::any_cast<std::string>(left) + '+' + std::to_string(std::any_cast<double>(reigth)) + ')';
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<int64_t>(left)) + '+' + std::any_cast<std::string>(reigth) + ')';
  else if(left.type() == typeid(double) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<double>(left)) + '+' + std::any_cast<std::string>(reigth) + ')';
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(int64_t)){
    int64_t ret = std::any_cast<int64_t>(left);
    for(int64_t i = std::any_cast<int64_t>(reigth); i >= 0; ++i)
      ret *= ret;
    return ret;
  }
  else if(left.type() == typeid(double) && reigth.type() == typeid(int64_t))
    return std::pow(std::any_cast<double>(left), std::any_cast<int64_t>(reigth));
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(double))
    return std::pow(std::any_cast<int64_t>(left), std::any_cast<double>(reigth));
  else if(left.type() == typeid(double) && reigth.type() == typeid(double))
    return std::pow(std::any_cast<double>(left), std::any_cast<double>(reigth));
  else{
    throw std::runtime_error("todo:"); //TODO:
  }
}

std::any MyVisitor::visitDim(SceneParser::DimContext *ctx){
  std::any left = ctx->children[0]->accept(this);
  std::any reigth = ctx->children[2]->accept(this);

  if(left.type() == typeid(std::string) && reigth.type() == typeid(std::string))
    return std::any_cast<std::string>(left) + '-' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(int64_t))
    return std::any_cast<std::string>(left) + '-' + std::to_string(std::any_cast<int64_t>(reigth));
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(double))
    return std::any_cast<std::string>(left) + '-' + std::to_string(std::any_cast<double>(reigth));
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<int64_t>(left)) + '-' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<double>(left)) + '-' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(int64_t))
    return std::any_cast<int64_t>(left) - std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(int64_t))
    return std::any_cast<double>(left) - std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(double))
    return std::any_cast<int64_t>(left) - std::any_cast<double>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(double))
    return std::any_cast<double>(left) - std::any_cast<double>(reigth);
  else{
    throw std::runtime_error("todo:"); //TODO:
  }
}

std::any MyVisitor::visitDife(SceneParser::DifeContext *ctx){
  std::any left = ctx->children[0]->accept(this);
  std::any reigth = ctx->children[2]->accept(this);

  if(left.type() == typeid(std::string) && reigth.type() == typeid(std::string))
    return std::any_cast<std::string>(left) + '/' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(int64_t))
    return std::any_cast<std::string>(left) + '/' + std::to_string(std::any_cast<int64_t>(reigth));
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(double))
    return std::any_cast<std::string>(left) + '/' + std::to_string(std::any_cast<double>(reigth));
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<int64_t>(left)) + '/' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<double>(left)) + '/' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(int64_t))
    return std::any_cast<int64_t>(left) / (double)std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(int64_t))
    return std::any_cast<double>(left) / std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(double))
    return std::any_cast<int64_t>(left) / std::any_cast<double>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(double))
    return std::any_cast<double>(left) / std::any_cast<double>(reigth);
  else{
    throw std::runtime_error("todo:"); //TODO:
  }
}

std::any MyVisitor::visitMult(SceneParser::MultContext *ctx){
  std::any left = ctx->children[0]->accept(this);
  std::any reigth = ctx->children[2]->accept(this);

  if(left.type() == typeid(std::string) && reigth.type() == typeid(std::string))
    return std::any_cast<std::string>(left) + '*' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(int64_t))
    return std::any_cast<std::string>(left) + '*' + std::to_string(std::any_cast<int64_t>(reigth));
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(double))
    return std::any_cast<std::string>(left) + '*' + std::to_string(std::any_cast<double>(reigth));
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<int64_t>(left)) + '*' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(std::string))
    return std::to_string(std::any_cast<double>(left)) + '*' + std::any_cast<std::string>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(int64_t))
    return std::any_cast<int64_t>(left) * std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(int64_t))
    return std::any_cast<double>(left) * std::any_cast<int64_t>(reigth);
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(double))
    return std::any_cast<int64_t>(left) * std::any_cast<double>(reigth);
  else if(left.type() == typeid(double) && reigth.type() == typeid(double))
    return std::any_cast<double>(left) * std::any_cast<double>(reigth);
  else{
    throw std::runtime_error("todo:"); //TODO:
  }
}

