#include "CodeGenerator.h"
#include "SceneParser.h"
#include <any>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>


std::any CodeGenerator::visitWalk(SceneParser::WalkContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  if( ret.type() == typeid(std::string)){

  }
  else if (ret.type() == typeid(int64_t)){

  }
  else if (ret.type() == typeid(double)){

  }
  else 
    throw std::runtime_error("todo"); 
}

std::any CodeGenerator::visitInt(SceneParser::IntContext *ctx){
  std::cout << __func__ << std::endl;
  int64_t ret = std::stoi(ctx->Num()->getSymbol()->getText());
  return ret;
}

std::any CodeGenerator::visitFloat(SceneParser::FloatContext *ctx){
  std::cout << __func__ << std::endl;
  double ret = std::stod(ctx->Float()->getSymbol()->getText());
  return ret;
}

std::any CodeGenerator::visitABS(SceneParser::ABSContext *ctx){
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

std::any CodeGenerator::visitNegate(SceneParser::NegateContext *ctx){
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

std::any CodeGenerator::visitNumExpr(SceneParser::NumExprContext *ctx){
  std::cout << __func__ << std::endl;
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

std::any CodeGenerator::visitAdd(SceneParser::AddContext *ctx){
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

std::any CodeGenerator::visitExp(SceneParser::ExpContext *ctx){
  std::cout << __func__ << std::endl;
  std::any left = ctx->children[0]->accept(this);
  std::any reigth = ctx->children[2]->accept(this);
  std::cout << "fisited child" << std::endl; // << left.type().name() << " " << reigth.type().name() << " " << typeid(int64_t).name() << std::endl;

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
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(int64_t))
    return std::pow(std::any_cast<int64_t>(left), std::any_cast<int64_t>(reigth));
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

std::any CodeGenerator::visitDim(SceneParser::DimContext *ctx){
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

std::any CodeGenerator::visitDife(SceneParser::DifeContext *ctx){
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

std::any CodeGenerator::visitMult(SceneParser::MultContext *ctx){
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

