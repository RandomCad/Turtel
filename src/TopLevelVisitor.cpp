#include "src/TopLevelVisitor.h"
#include "SceneParser.h"
#include "CodeGenerator.Helper.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/VariableHeandler.h"

#include <any>
#include <cstdint>
#include <ostream>
#include <string>

///define function to unpack expr return
std::string TopLevelVisitor::UnwrapExpre(SceneParser::ExprContext *ctx){
  std::any ret = ctx->accept(&mathVis);
  if(ret.type() == typeid(std::string))     return std::any_cast<std::string>(ret);
  else if (ret.type() == typeid(int64_t))   return std::to_string(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))   return std::to_string(std::any_cast<double>(ret));
  else{
    throw "Error unknowen type";
  }
}

std::any TopLevelVisitor::visitStopOK(SceneParser::StopOKContext *ctx){
  output 
    << "  __envfunc_fin(0, " << vars.getVariableNameString(RND_NAME) << ");\n";
  return std::any();
}

std::any TopLevelVisitor::visitStopError(SceneParser::StopErrorContext *ctx){
  output 
    << "  __envfunc_fin("
    << UnwrapExpre(ctx->expr())
    <<", " << vars.getVariableNameString(RND_NAME) << ");\n";
  return std::any();
}

std::any TopLevelVisitor::visitClear(SceneParser::ClearContext *ctx){
  output  << "  SDL_RenderClear("
          << vars.getVariableNameString(RND_NAME)
          << ");\n"
          ;
  return std::any();
}

std::any TopLevelVisitor::visitDirection(SceneParser::DirectionContext *ctx){
  std::any ret = ctx->expr()->accept(&mathVis);
  if(ret.type() == typeid(std::string)){ //TODO clamp (what if always turning to the rigth -> pressision of the double gets worse.
    output  << "  " 
            << vars.getVariableNameString(ROTATION) 
            << " = ("
            << std::any_cast<std::string>(ret)
            << ") * (M_PI/180);\n"
            ;
  }
  else if (ret.type() == typeid(int64_t)){
    output  << "  " 
            << vars.getVariableNameString(ROTATION) 
            << " = ("
            << std::any_cast<int64_t>(ret)
            << ") * (M_PI/180);\n"
            ;
  }
  else if (ret.type() == typeid(double)){
    output  << "  " 
            << vars.getVariableNameString(ROTATION) 
            << " = ("
            << std::any_cast<double>(ret)
            << ") * (M_PI/180);\n"
            ;  
  }
  else{
    throw "Error unknowen type";
  }
  
  return std::any();

}

std::any TopLevelVisitor::visitTurnRight(SceneParser::TurnRightContext *ctx){
  std::any ret = ctx->expr()->accept(&mathVis);
  if(ret.type() == typeid(std::string)){ //TODO clamp (what if always turning to the rigth -> pressision of the double gets worse.
    output  << "  " 
            << vars.getVariableNameString(ROTATION) 
            << " += ("
            << std::any_cast<std::string>(ret)
            << ") * (M_PI/180);\n"
            ;
  }
  else if (ret.type() == typeid(int64_t)){
    output  << "  " 
            << vars.getVariableNameString(ROTATION) 
            << " += ("
            << std::any_cast<int64_t>(ret)
            << ") * (M_PI/180);\n"
            ;
  }
  else if (ret.type() == typeid(double)){
    output  << "  " 
            << vars.getVariableNameString(ROTATION) 
            << " += ("
            << std::any_cast<double>(ret)
            << ") * (M_PI/180);\n"
            ;  
  }
  else{
    throw "Error unknowen type";
  }
  
  return std::any();
}

///go back to WINDOW_X/2 and WINDOW_Y which should be the middle of the bottom of the screen
std::any TopLevelVisitor::visitWaklHome(SceneParser::WaklHomeContext *ctx){
  output  << "  SDL_RenderDrawLine("
          << vars.getVariableNameString(RND_NAME) 
          << ", " 
          << vars.getVariableNameString(POS_X) 
          << ", "
          << vars.getVariableNameString(POS_Y) 
          << ", "
          << vars.getVariableNameString(WINDOW_X) << " / 2, "
          << vars.getVariableNameString(WINDOW_Y) 
          << ");\n"
          
          << vars.getVariableNameString(POS_X) 
          << " = " 
          << vars.getVariableNameString(WINDOW_X) 
          << " / 2;\n"

          << vars.getVariableNameString(POS_Y) 
          << " = " 
          << vars.getVariableNameString(WINDOW_Y) 
          << ";\n"
          ;

  return std::any();
}

///go back to WINDOW_X/2 and WINDOW_Y which should be the middle of the bottom of the screen
std::any TopLevelVisitor::visitJumpHome(SceneParser::JumpHomeContext *ctx){
  output  << vars.getVariableNameString(POS_X) 
          << " = " 
          << vars.getVariableNameString(WINDOW_X) 
          << " / 2;\n"

          << vars.getVariableNameString(POS_Y) 
          << " = " 
          << vars.getVariableNameString(WINDOW_Y) 
          << ";\n"
          ;

  return std::any();
}

///File local funtion to move POS_X and POS_Y
void MovePositions(VariableHeandler &vars, std::ostream &output, std::any &ret){
  if(ret.type() == typeid(std::string)){
    output << "  " << vars.getVariableNameString(POS_X) << " = ";
    CalcPosX(std::any_cast<std::string>(ret), output, vars);
    output 
      << ";\n"
      
      << "  " << vars.getVariableNameString(POS_Y) << " = ";
    CalcPosY(std::any_cast<std::string>(ret), output, vars);
    output << ";\n";
  }
  else if (ret.type() == typeid(int64_t)){
    output << "  " << vars.getVariableNameString(POS_X) << " = ";
    std::cout << "got an int" << std::endl;
    CalcPosX(std::any_cast<int64_t>(ret), output, vars);
    output 
      << ";\n"
      
      << "  " << vars.getVariableNameString(POS_Y) << " = ";
    CalcPosY(std::any_cast<int64_t>(ret), output, vars);
    output << ";\n";
  }
  else if (ret.type() == typeid(double)){
    output << "  " << vars.getVariableNameString(POS_X) << " = ";
    CalcPosX(std::any_cast<double>(ret), output, vars);
    output 
      << ";\n"
      
      << "  " << vars.getVariableNameString(POS_Y) << " = ";
    CalcPosY(std::any_cast<double>(ret), output, vars);
    output << ";\n";
  }
  else{
    throw "Error unknowen type";
  }
}
/**
 *Expected output:
 *SDL_RenderDrawLine(@renderVar, @x, @y, @x + divx, @y + divy
 * */
std::any TopLevelVisitor::visitWalkFront(SceneParser::WalkFrontContext *ctx){
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
  output << ");\n";
  MovePositions(vars, output, ret);
  GenPresent(vars, output);
  return std::any();
}

std::any TopLevelVisitor::visitJumpFront(SceneParser::JumpFrontContext *ctx){
  std::any ret = ctx->expr()->accept(&mathVis);

  MovePositions(vars, output, ret);
  
  return std::any();
}

std::any TopLevelVisitor::visitSave(SceneParser::SaveContext *ctx){
  output
    << "  save_texture(\"" 
      << ctx->ID()->getText() << ".png\", "
      << vars.getVariableNameString(RND_NAME) << ", "
      << vars.getVariableNameString(TEXTURE_NAME) 
    << ");\n"
    ;
  return  std::any();
}

