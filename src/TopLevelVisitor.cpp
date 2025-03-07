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
#include <cmath>
#include <cassert>

///define function to unpack expr return
std::string TopLevelVisitor::UnwrapExpre(SceneParser::ExprContext *ctx){
  std::any ret = ctx->accept(this);
  if(ret.type() == typeid(std::string))     return std::any_cast<std::string>(ret);
  else if (ret.type() == typeid(int64_t))   return std::to_string(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))   return std::to_string(std::any_cast<double>(ret));
  else{
    std::cerr << "unknowen type: " << ret.type().name() << std::endl;
    throw "Error unknowen type";
  }
}

std::any TopLevelVisitor::visitVariable(SceneParser::VariableContext *ctx){
  std::cerr << __func__ << std::endl;
  return ctx->ID()->getText();
}
std::any TopLevelVisitor::visitGlobalVariable(SceneParser::GlobalVariableContext *ctx){
  std::cerr << __func__ << std::endl;
  return ctx->IncID()->getText();
}

std::any TopLevelVisitor::visitStoreVar(SceneParser::StoreVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << vars.getVariableNameString(std::any_cast<std::string>(ctx->var()->accept(this)))
    << " = "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}

std::any TopLevelVisitor::visitSubVar(SceneParser::SubVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << vars.getVariableNameString(std::any_cast<std::string>(ctx->var()->accept(this)))
    << " -= "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}

std::any TopLevelVisitor::visitDivVar(SceneParser::DivVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << vars.getVariableNameString(std::any_cast<std::string>(ctx->var()->accept(this)))
    << " /= "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}

std::any TopLevelVisitor::visitAddVar(SceneParser::AddVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << vars.getVariableNameString(std::any_cast<std::string>(ctx->var()->accept(this)))
    << " += "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}

std::any TopLevelVisitor::visitMultVar(SceneParser::MultVarContext *ctx){
  output
    << vars.getVariableNameString(std::any_cast<std::string>(ctx->var()->accept(this)))
    << " *= "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}

std::any TopLevelVisitor::visitFinError(SceneParser::FinErrorContext *ctx){
  output 
    << "  __envfunc_fin("
    << UnwrapExpre(ctx->expr())
    <<", " << vars.getVariableNameString(RND_NAME) << ");\n";
  return std::any();

}

std::any TopLevelVisitor::visitFinOK(SceneParser::FinOKContext *ctx){
  output 
    << "  __envfunc_fin(0, " << vars.getVariableNameString(RND_NAME) << ");\n";
  return std::any();
}

std::any TopLevelVisitor::visitStopOK(SceneParser::StopOKContext *ctx){
  output 
    << "  __envfunc_stop(0, " << vars.getVariableNameString(RND_NAME) << ");\n";
  return std::any();
}

std::any TopLevelVisitor::visitStopError(SceneParser::StopErrorContext *ctx){
  output 
    << "  __envfunc_stop("
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
  output  << "  " 
          << vars.getVariableNameString(ROTATION) 
          << " = ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}

std::any TopLevelVisitor::visitTurnRight(SceneParser::TurnRightContext *ctx){
  output  << "  " 
          << vars.getVariableNameString(ROTATION) 
          << " += ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}
std::any TopLevelVisitor::visitTurnLeft(SceneParser::TurnLeftContext *ctx){
  output  << "  " 
          << vars.getVariableNameString(ROTATION) 
          << " -= ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}

///go back to WINDOW_X/2 and WINDOW_Y which should be the middle of the bottom of the screen
std::any TopLevelVisitor::visitWalkHome(SceneParser::WalkHomeContext *ctx){
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
void MovePositions(VariableHeandler &vars, std::ostream &output, std::string &&ret){
  output << "  " << vars.getVariableNameString(POS_X) << " = ";
  CalcPosX(ret, output, vars);
  output 
    << ";\n"
    
    << "  " << vars.getVariableNameString(POS_Y) << " = ";
  CalcPosY(ret, output, vars);
  output << ";\n";
}
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
  std::any ret = ctx->expr()->accept(this);
  output  << "  SDL_RenderDrawLine("
          << vars.getVariableNameString(RND_NAME) 
          << ", " 
          << vars.getVariableNameString(POS_X) 
          << ", "
          << vars.getVariableNameString(POS_Y) 
          << ", "
          ;
  CalcPosX(UnwrapExpre(ctx->expr()), output, vars);
  output  << ",";
  CalcPosY(UnwrapExpre(ctx->expr()), output, vars);
  output  << ");\n";
  MovePositions(vars, output, ret);
  GenPresent(vars, output);
  return std::any();
}
std::any TopLevelVisitor::visitWalkBack(SceneParser::WalkBackContext *ctx){
  output  << "  SDL_RenderDrawLine("
          << vars.getVariableNameString(RND_NAME) 
          << ", " 
          << vars.getVariableNameString(POS_X) 
          << ", "
          << vars.getVariableNameString(POS_Y) 
          << ", "
          ;
  CalcPosX("-(" + UnwrapExpre(ctx->expr()) + ')', output, vars);
  output  << ",";
  CalcPosY("-(" + UnwrapExpre(ctx->expr()) + ')', output, vars);
  output  << ");\n";
  MovePositions(vars, output, "-(" + UnwrapExpre(ctx->expr()) + ')');
  GenPresent(vars, output);
  return std::any();
}

std::any TopLevelVisitor::visitJumpFront(SceneParser::JumpFrontContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  MovePositions(vars, output, ret);
  
  return std::any();
}
std::any TopLevelVisitor::visitJumpBack(SceneParser::JumpBackContext *ctx){
  MovePositions(vars, output, "-(" + UnwrapExpre(ctx->expr()) + ')');
  
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

std::any TopLevelVisitor::visitMark(SceneParser::MarkContext *ctx) {
  output << "  pushMarker((Marker){ "
         << vars.getVariableNameString(POS_X) << ", "
         << vars.getVariableNameString(POS_Y) << ", "
         << vars.getVariableNameString(ROTATION)
         << " });\n";
  return std::any();
}

std::any TopLevelVisitor::visitWalkMark(SceneParser::WalkMarkContext *ctx) {
  output << "  if (markerStackTop >= 0) {\n"
  	     << "      Marker m = popMarker();\n"
  	     << "      SDL_RenderDrawLine(" 
  	     << vars.getVariableNameString(RND_NAME) << ", "
  	     << vars.getVariableNameString(POS_X) << ", "
  	     << vars.getVariableNameString(POS_Y) << ", "
  	     << "m.posX, m.posY);\n"
  	     << "      " << vars.getVariableNameString(POS_X) << " = m.posX;\n"
  	     << "      " << vars.getVariableNameString(POS_Y) << " = m.posY;\n"
  	     << "      " << vars.getVariableNameString(ROTATION) << " = m.rotation;\n"
  	     << "  } else {\n"
  	     << "      fprintf(stderr, \"Fehler: Marker-Stack leer!\\n\");\n"
  	     << "      exit(EXIT_FAILURE);\n"
  	     << "  }\n";
  return std::any();
}

std::any TopLevelVisitor::visitJumpMark(SceneParser::JumpMarkContext *ctx) {
  output << "  if (markerStackTop >= 0) {\n"
         << "      Marker m = popMarker();\n"
         << "      " << vars.getVariableNameString(POS_X) << " = m.posX;\n"
         << "      " << vars.getVariableNameString(POS_Y) << " = m.posY;\n"
         << "      " << vars.getVariableNameString(ROTATION) << " = m.rotation;\n"
         << "  } else {\n"
         << "      fprintf(stderr, \"Fehler: Marker-Stack leer!\\n\");\n"
         << "      exit(EXIT_FAILURE);\n"
         << "  }\n";
  return std::any();
}

std::any TopLevelVisitor::visitColorCmd(SceneParser::ColorCmdContext *ctx) {

  std::string rValue = UnwrapExpre(ctx->expr(0));
  std::string gValue = UnwrapExpre(ctx->expr(1));
  std::string bValue = UnwrapExpre(ctx->expr(2));

  output << "  SDL_SetRenderDrawColor(" 
         << vars.getVariableNameString(RND_NAME) << ", "
         << rValue << ", "
         << gValue << ", "
         << bValue << ", 255);\n";
  
  output << "  " << vars.getVariableNameString(COLOR_R) << " = " << rValue << ";\n"
         << "  " << vars.getVariableNameString(COLOR_G) << " = " << gValue << ";\n"
         << "  " << vars.getVariableNameString(COLOR_B) << " = " << bValue << ";\n";

  return std::any();
}




std::any TopLevelVisitor::visitInt(SceneParser::IntContext *ctx){
  std::cerr << __func__ << std::endl;
  int64_t ret = std::stol(ctx->Num()->getSymbol()->getText());
  std::cerr << ret << std::endl;
  return ret;
}

std::any TopLevelVisitor::visitFloat(SceneParser::FloatContext *ctx){
  std::cerr << __func__ << std::endl;
  return std::stod(ctx->Float()->getSymbol()->getText());
}

#define OperationMacro(symbol) \
  std::cout << __func__ << std::endl;\
  std::any left = ctx->children[0]->accept(this);\
  std::any reigth = ctx->children[2]->accept(this);\
  if(left.type() == typeid(std::string) && reigth.type() == typeid(std::string))\
    return std::any_cast<std::string>(left) + #symbol + std::any_cast<std::string>(reigth);\
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(int64_t))\
    return std::any_cast<std::string>(left) + #symbol + std::to_string(std::any_cast<int64_t>(reigth));\
  else if(left.type() == typeid(std::string) && reigth.type() == typeid(double))\
    return std::any_cast<std::string>(left) + #symbol + std::to_string(std::any_cast<double>(reigth));\
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(std::string))\
    return std::to_string(std::any_cast<int64_t>(left)) + #symbol + std::any_cast<std::string>(reigth);\
  else if(left.type() == typeid(double) && reigth.type() == typeid(std::string))\
    return std::to_string(std::any_cast<double>(left)) + #symbol + std::any_cast<std::string>(reigth);\
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(int64_t))\
    return std::any_cast<int64_t>(left) symbol std::any_cast<int64_t>(reigth);\
  else if(left.type() == typeid(double) && reigth.type() == typeid(int64_t))\
    return std::any_cast<double>(left) symbol std::any_cast<int64_t>(reigth);\
  else if(left.type() == typeid(int64_t) && reigth.type() == typeid(double))\
    return std::any_cast<int64_t>(left) symbol std::any_cast<double>(reigth);\
  else if(left.type() == typeid(double) && reigth.type() == typeid(double))\
    return std::any_cast<double>(left) symbol std::any_cast<double>(reigth);\
  else{\
    throw std::runtime_error("todo:"); /*TODO:*/\
  }
///returns a string or bool
std::any TopLevelVisitor::visitLesEqThan(SceneParser::LesEqThanContext *ctx){
  OperationMacro(<=);  
}

///returns a string or bool
std::any TopLevelVisitor::visitGreaterThan(SceneParser::GreaterThanContext *ctx){
  OperationMacro(>);  
}

///returns a string or bool
std::any TopLevelVisitor::visitLesThan(SceneParser::LesThanContext *ctx){
  OperationMacro(<);  
}

///returns a string or bool
std::any TopLevelVisitor::visitGreaterEqThan(SceneParser::GreaterEqThanContext *ctx){
  OperationMacro(<=);  
}

std::any TopLevelVisitor::visitClamCond(SceneParser::ClamCondContext *ctx){
  std::any ret = ctx->cond();
  if (ret.type() == typeid(bool))
    return ret;
  else if (ret.type() == typeid(std::string))
    return '(' + std::any_cast<std::string>(ret) + ')';
  else
   throw "TODO"; //TODO:
}

std::any TopLevelVisitor::visitNotCond(SceneParser::NotCondContext *ctx){
  std::any ret = ctx->cond();
  if (ret.type() == typeid(bool))
    return !std::any_cast<bool>(ret);
  else if (ret.type() == typeid(std::string))
    return "!(" + std::any_cast<std::string>(ret) + ')';
  else
   throw "TODO"; //TODO:
}

std::any TopLevelVisitor::visitAndCond(SceneParser::AndCondContext *ctx){
  OperationMacro(&&);
}

std::any TopLevelVisitor::visitOrCond(SceneParser::OrCondContext *ctx){
  OperationMacro(||);
}

std::any TopLevelVisitor::visitABS(SceneParser::ABSContext *ctx){
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

std::any TopLevelVisitor::visitNegate(SceneParser::NegateContext *ctx){
  assert(ctx->children.size() == 2);
  std::any number = ctx->children[1]->accept(this);
  if (number.type() == typeid(int64_t)){
    return -std::any_cast<int64_t>(number);
  }
  else if (number.type() == typeid(double)){
    return -std::any_cast<double>(number);
  }
  else if (number.type() == typeid(std::string)) {
    return "-" + std::any_cast<std::string>(number);
  }
  else{
    throw std::runtime_error("coudn't cast number context to number");
  }
}

std::any TopLevelVisitor::visitNumExpr(SceneParser::NumExprContext *ctx){
  std::cout << __func__ << std::endl;
  return ctx->number()->accept(this);
}

std::any TopLevelVisitor::visitAdd(SceneParser::AddContext *ctx){
  std::cout << __func__ << std::endl;
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

std::any TopLevelVisitor::visitExp(SceneParser::ExpContext *ctx){
  std::cout << __func__ << std::endl;
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

std::any TopLevelVisitor::visitDim(SceneParser::DimContext *ctx){
  std::cout << __func__ << std::endl;
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

std::any TopLevelVisitor::visitDife(SceneParser::DifeContext *ctx){
  std::cout << __func__ << std::endl;
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

std::any TopLevelVisitor::visitMult(SceneParser::MultContext *ctx){
  std::cout << __func__ << std::endl;
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

std::any TopLevelVisitor::visitVarExpr(SceneParser::VarExprContext *ctx){
  return vars.getVariableNameString(std::any_cast<std::string>(ctx->var()->accept(this)));
}
