#include "src/TopLevelVisitor.h"
#include "SceneParser.h"
#include "CodeGenerator.Helper.h"

#include <any>

std::any TopLevelVisitor::visitWalk(SceneParser::WalkContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  output  << "  SDL_RenderDrawLine(rnd, " 
          << vars.getVariableNameString("@x") 
          << ", "
          << vars.getVariableNameString("@y") 
          << ", ";
  if(ret.type() == typeid(std::string)){
    CalcPosX(std::any_cast<std::string>(ret), output, vars);
    output << ", ";
    CalcPosY(std::any_cast<std::string>(ret), output, vars);
  }
  else if (ret.type() == typeid(int64_t)){
    CalcPosX(std::any_cast<int64_t>(ret), output, vars);
    output << ", ";
    CalcPosY(std::any_cast<int64_t>(ret), output, vars);
  }
  else if (ret.type() == typeid(double)){
    CalcPosX(std::any_cast<double>(ret), output, vars);
    output << ", ";
    CalcPosY(std::any_cast<double>(ret), output, vars);
  }
  else{
    throw "Error unknowen type";
  }
  output 
    << ");"
    << std::endl;
  return std::any();
}
