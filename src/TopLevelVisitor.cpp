#include "src/TopLevelVisitor.h"
#include "SceneParser.h"
#include "CodeGenerator.Helper.h"
#include "src/InternalVarNames.h"

#include <any>

/**
 *Expected output:
 *SDL_RenderDrawLine(@renderVar, @x, @y, @x + divx, @y + divy
 * */
std::any TopLevelVisitor::visitWalk(SceneParser::WalkContext *ctx){
  std::any ret = ctx->expr()->accept(&mathVis);
  output  << "  SDL_RenderDrawLine("
          << vars.getVariableNameString(RND_NAME) 
          << ", " 
          << vars.getVariableNameString(POS_X) 
          << ", "
          << vars.getVariableNameString(POS_Y) 
          << ", ";

  if(ret.type() == typeid(std::string)){
    CalcPosX(std::any_cast<std::string>(ret), output, vars);
    output << ", ";
    CalcPosY(std::any_cast<std::string>(ret), output, vars);
  }
  else if (ret.type() == typeid(int64_t)){
    std::cout << "got an int" << std::endl;
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
