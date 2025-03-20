#include "src/TopLevelVisitor.h"
#include "SceneParser.h"
#include "CodeGenerator.Helper.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/VariableHeandler.h"

#include <any>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <ostream>
#include <string>
#include <cmath>
#include <cassert>

int TopLevelVisitor::infinitLoopFlag = 0;

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

std::any TopLevelVisitor::visitIf(SceneParser::IfContext *ctx){
  std::any ret = ctx->cond()->accept(this);
  if(ret.type() == typeid(bool)){
    if(!std::any_cast<bool>(ret)){
      if(ctx->else_()){
        output << "//if was optimiced out cond was false only generating else branche\n";
        ///inlinded visit else
        for (auto i : ctx->else_()->stat()) {
          i->accept(this);
        }
      }
      else{
        output << "//if was optimiced out cond was false no else branch\n";
      }
      return std::any();
    }
    else{
      output << "//if was optimiced out cond was true else branch is irrelevant\n";
      for (auto i : ctx->stat()) {
        i->accept(this);
      }
      return std::any();
    }
  }
  else if (ret.type() == typeid(std::string)){
    output << "  if (" << std::any_cast<std::string>(ret) << "){\n";
    for (auto i : ctx->stat()) {
      i->accept(this);
    }
    if(ctx->else_()){
      output  << "  }\n"
              << "  else{\n";
      for (auto i : ctx->else_()->stat()) {
        i->accept(this);
      }
    }
    output  << "  }\n";
    return std::any();
  }
  else{
    throw "todo"; //TODO
  }
}

std::any TopLevelVisitor::visitWhile(SceneParser::WhileContext *ctx){
  std::any ret = ctx->cond()->accept(this);
  if(ret.type() == typeid(bool)){
    if(std::any_cast<bool>(ret)){
      std::string usrInput;
      switch (infinitLoopFlag) {
        case 1:
          usrInput = "y";
          break;;
        case 0:
          do{ //TODO add flagsupport
            std::cerr 
              << "you created an infinit loop at line: " 
              << ctx->getTokens(SceneParser::While)[0]->getSymbol()->getLine() 
              << "starting with character: "
              << ctx->getTokens(SceneParser::While)[0]->getSymbol()->getCharPositionInLine()
              << ".\n"
              
              << "This while wil not be excaped once enterde.\n"
              << "Continue generating? (y/n)" 
              << std::endl;
            std::cin >> usrInput;
          } while(usrInput.length() == 2);
          break;
        case -1:
          usrInput = "n";
          break;
      }
      switch (usrInput[0]) {
        case 'n':
          std::cerr << "Ending programm" << std::endl;
          std::exit(1);
        default:
          ///retry is hopfully optimized
          return ctx->accept(this);
        case 'y':
          output  << "  //user wants an infinit loop!\n"
                  << " while (1) {\n";
          for (auto i : ctx->stat()) {
            i->accept(this);
          }
          output << "  }\n";
          return std::any();
      }
    }
    else{
      output << "//while was optimized out the condition was false\n";
    }
  }
  else if(ret.type() == typeid(std::string)){
    output << "  while (" << std::any_cast<std::string>(ret) << "){\n";
    for (auto i : ctx->stat()) {
      i->accept(this);
    }
    output << "  }\n";
  }
  else{
    throw "TODO"; //TODO
  }
  return std::any();
}

std::any TopLevelVisitor::visitDoUntil(SceneParser::DoUntilContext *ctx){
  std::cerr << __func__ << std::endl;
  std::any ret = ctx->cond()->accept(this);
  if(ret.type() == typeid(bool)){
    if(!std::any_cast<bool>(ret)){
      std::string usrInput;
      switch (infinitLoopFlag) {
        case 1:
          usrInput = "y";
          break;;
        case 0:
          do{ //TODO add flagsupport
            std::cerr 
              << "you created an infinit loop at line: " 
              << ctx->getTokens(SceneParser::Untile)[0]->getSymbol()->getLine() 
              << "starting with character: "
              << ctx->getTokens(SceneParser::Untile)[0]->getSymbol()->getCharPositionInLine()
              << ".\n"
              
              << "This repeat untill wil not be escaped once enterde.\n"
              << "Continue generating? (y/n)" 
              << std::endl;
            std::cin >> usrInput;
          } while(usrInput.length() == 2);

          break;
        case -1:
          usrInput = "n";
          break;
      }
      
      switch (usrInput[0]) {
        case 'n':
          std::cerr << "Ending programm" << std::endl;
          std::exit(1);
        default:
          ///retry is hopfully optimized
          return ctx->accept(this);
        case 'y':
          output  << "  //user wants an infinit loop!\n"
                  << " do {\n";
          for (auto i : ctx->stat()) {
            i->accept(this);
          }
          output << "  }while(1);\n";
          return std::any();
      }
    }
    else{
      for (auto i : ctx->stat()) {
        i->accept(this);
      }
      output << "//repeat until was optimized out the condition was true\n";
    }
  }
  else if(ret.type() == typeid(std::string)){
    output << "  do{\n";
    for (auto i : ctx->stat()) {
      i->accept(this);
    }
    output << "  }while (!(" << std::any_cast<std::string>(ret) << "));\n";
  }
  else{
    throw "TODO"; //TODO
  }
  return std::any();
}

std::any TopLevelVisitor::visitToFor(SceneParser::ToForContext *ctx) { //TODO case to is negativ!
  std::any ret = ctx->expr()->accept(this);
  if(ret.type() == typeid(double)) ret = (int64_t)std::ceil(std::any_cast<double>(ret));
  if (ret.type() == typeid(int64_t)){
    output  << "#pragma unroll\n"
            << "  for (size_t i = 0; i <" << std::any_cast<int64_t>(ret) << " ; ++i){\n"
            ;
    for (auto i : ctx->stat()) {
      i->accept(this);
    }
    output  << "}\n";
  }
  else if (ret. type() == typeid(std::string)){
    output  << "  for (size_t i = 0; i <" << std::any_cast<std::string>(ret) << " ; ++i){\n"
            ;
    for (auto i : ctx->stat()) {
      i->accept(this);
    }
    output  << "}\n";
  }
  else{
    throw "TODO"; //TODO;
  }
  return std::any();
}

std::any TopLevelVisitor::visitSimpUpFor(SceneParser::SimpUpForContext *ctx){
  std::cerr << "ctx1:" << std::endl;
  std::any from = ctx->children[3]->accept(this);
  std::cerr << "ctx1:" << std::endl;
  std::any to = ctx->children[5]->accept(this);
  std::cerr << "rest" << std::endl;

  if( (from.type() == typeid(double) || from.type() == typeid(int64_t)) &&
      (to.type() == typeid(double) || to.type() == typeid(int64_t))){
    output << "#pragma unroll\n";
  }
  output  << "  for ( " 
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " < "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; ++"
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << "){\n"
          ;

  for (auto i : ctx->stat()) {
    i->accept(this);
  }

  output  << "  }\n";

  return std::any();
}
std::any TopLevelVisitor::visitSimpDownFor(SceneParser::SimpDownForContext *ctx){
  std::any from = ctx->children[3]->accept(this);
  std::any to = ctx->children[5]->accept(this);

  if( (from.type() == typeid(double) || from.type() == typeid(int64_t)) &&
      (to.type() == typeid(double) || to.type() == typeid(int64_t))){
    output << "#pragma unroll\n";
  }
  output  << "  for ( " 
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " > "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; --"
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << "){\n"
          ;

  for (auto i : ctx->stat()) {
    i->accept(this);
  }

  output  << "  }\n";

  return std::any();
}
std::any TopLevelVisitor::visitStepDownFor(SceneParser::StepDownForContext *ctx){
  std::any from = ctx->children[3]->accept(this);
  std::any to = ctx->children[5]->accept(this);
  std::any step = ctx->children[7]->accept(this);

  if( (from.type() == typeid(double) || from.type() == typeid(int64_t)) &&
      (to.type() == typeid(double) || to.type() == typeid(int64_t)) &&
      (step.type() == typeid(double) || step.type() == typeid(int64_t))){
    output << "#pragma unroll\n";
  }
  output  << "  for ( " 
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " > "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; "
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " -= "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[7]))
          << "){\n"
          ;

  for (auto i : ctx->stat()) {
    i->accept(this);
  }

  output  << "  }\n";

  return std::any();
}
std::any TopLevelVisitor::visitStepUpFor(SceneParser::StepUpForContext *ctx){
  std::any from = ctx->children[3]->accept(this);
  std::any to = ctx->children[5]->accept(this);
  std::any step = ctx->children[7]->accept(this);

  if( (from.type() == typeid(double) || from.type() == typeid(int64_t)) &&
      (to.type() == typeid(double) || to.type() == typeid(int64_t)) &&
      (step.type() == typeid(double) || step.type() == typeid(int64_t))){
    output << "#pragma unroll\n";
  }
  output  << "  for ( " 
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " < "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; "
          << std::any_cast<std::string>(ctx->var()->accept(this))
          << " += "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[7]))
          << "){\n"
          ;

  for (auto i : ctx->stat()) {
    i->accept(this);
  }

  output  << "  }\n";

  return std::any();
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
std::any TopLevelVisitor::visitUnequal(SceneParser::UnequalContext *ctx){
  OperationMacro(!=);  
}
///returns a string or bool
std::any TopLevelVisitor::visitEqual(SceneParser::EqualContext *ctx){
  OperationMacro(==);  
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
  OperationMacro(>=);  
}

std::any TopLevelVisitor::visitClamCond(SceneParser::ClamCondContext *ctx){
  std::cerr << __func__ << std::endl;
  std::any ret = ctx->cond()->accept(this);
  if (ret.type() == typeid(bool))
    return ret;
  else if (ret.type() == typeid(std::string))
    return '(' + std::any_cast<std::string>(ret) + ')';
  else
   throw "TODO"; //TODO:
}

std::any TopLevelVisitor::visitNotCond(SceneParser::NotCondContext *ctx){
  std::any ret = ctx->cond()->accept(this);
  if (ret.type() == typeid(bool))
    return !std::any_cast<bool>(ret);
  else if (ret.type() == typeid(std::string))
    return "!(" + std::any_cast<std::string>(ret) + ')';
  else
   throw "TODO"; //TODO:
}

std::any TopLevelVisitor::visitAndCond(SceneParser::AndCondContext *ctx){
  std::cerr << __func__ << std::endl;
  std::any left = ctx->children[0]->accept(this);
  std::any right = ctx->children[2]->accept(this);
  if (left.type() == typeid(bool) && right.type() == typeid(bool))
      return std::any_cast<bool>(left) && std::any_cast<bool>(right);
  else if (left.type() == typeid(bool) && right.type() == typeid(std::string))
  {
    if(std::any_cast<bool>(left)) 
      return std::any_cast<std::string>(right);
    else
     return false;
  }
  else if (left.type() == typeid(std::string) && right.type() == typeid(bool))
  {
    if(std::any_cast<bool>(right)) 
      return std::any_cast<std::string>(left);
    else
     return false;
  }
  else if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
    return std::any_cast<std::string>(left) + "&&" + std::any_cast<std::string>(right);
  else
   throw "todo"; //TODO
}

std::any TopLevelVisitor::visitOrCond(SceneParser::OrCondContext *ctx){
  std::cerr << __func__ << std::endl;
  std::any left = ctx->children[0]->accept(this);
  std::any right = ctx->children[2]->accept(this);
  if (left.type() == typeid(bool) && right.type() == typeid(bool))
      return std::any_cast<bool>(left) || std::any_cast<bool>(right);
  else if (left.type() == typeid(bool) && right.type() == typeid(std::string))
  {
    if(std::any_cast<bool>(left)) 
      return true;
    else
      return std::any_cast<std::string>(right);
  }
  else if (left.type() == typeid(std::string) && right.type() == typeid(bool))
  {
    if(std::any_cast<bool>(right)) 
      return true;
    else
      return std::any_cast<std::string>(left);
  }
  else if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
    return std::any_cast<std::string>(left) + "||" + std::any_cast<std::string>(right);
  else
   throw "todo"; //TODO
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
