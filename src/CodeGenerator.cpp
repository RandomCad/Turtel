#include "CodeGenerator.h"
#include "../libs/SceneParser.h"
#include <any>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
std::any CodeGenerator::visitMain(SceneParser::MainContext *ctx){
  output
    << "void " "TurtelMain(SDL_Renderer * " RENDERER_NAME "){\n"
    << "  " << _variables.getVariableDefinition("@x") << ";\n"
    << "  " << _variables.getVariableDefinition("@y") << ";\n";

  //visit all the contained statments(stat)
  for(auto i : ctx->children) i->accept(this);

  output
    << "}\n" 
    <<std::endl;
  return nullptr;
}

std::any CodeGenerator::visitWalk(SceneParser::WalkContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  output << "  SDL_RenderDrawLine(rnd, POSITION_X_NAME, POSITION_Y_NAME,";
  if(ret.type() == typeid(std::string)){
    CalcPosX(std::any_cast<std::string>(ret));
    output << ", ";
    CalcPosY(std::any_cast<std::string>(ret));
  }
  else if (ret.type() == typeid(int64_t)){
    CalcPosX(std::any_cast<int64_t>(ret));
    output << ", ";
    CalcPosY(std::any_cast<int64_t>(ret));
  }
  else if (ret.type() == typeid(double)){
    CalcPosX(std::any_cast<double>(ret));
    output << ", ";
    CalcPosY(std::any_cast<double>(ret));
  }
  output 
    << ");"
    << std::endl;
  return std::any();
}
