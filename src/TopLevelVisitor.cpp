#include "src/TopLevelVisitor.h"
#include "SceneParser.h"
#include "GlobalVarVisitor.h"
#include "VariableVisitor.h"
#include "src/InternalVarNames.h"
#include "src/VariableHeandler.h"
#include "src/FunctionHandler.h"
#include "src/CodeGenerator.Helper.h"

#include <algorithm>
#include <any>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>
#include <cmath>
#include <cassert>
#include <vector>

#define GenPresent \
     "SDL_SetRenderTarget(" << envVar[RND_NAME].getName() << ", NULL);\n" \
    << "SDL_RenderClear( " << envVar[RND_NAME].getName() << ");\n" \
    << "SDL_RenderCopy(" << envVar[RND_NAME].getName() << ", " << envVar[TEXTURE_NAME].getName() << ", NULL, NULL);\n"\
    << "SDL_RenderPresent( " << envVar[RND_NAME].getName() << ");\n" \
    << "SDL_SetRenderTarget(" << envVar[RND_NAME].getName() << ", " << envVar[TEXTURE_NAME].getName() << ");\n" 


int TopLevelVisitor::infinitLoopFlag = 0;

TopLevelVisitor::TopLevelVisitor(std::ostream &a): output(a) {}

///define function to unpack expr return
std::string TopLevelVisitor::UnwrapExpre(SceneParser::ExprContext *ctx){
  std::any ret = ctx->accept(this);
  if(ret.type() == typeid(std::string))     return std::any_cast<std::string>(ret);
  else if (ret.type() == typeid(int64_t))   return std::to_string(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))    return std::to_string(std::any_cast<double>(ret));
  else{
    std::cerr << "unknowen type: " << ret.type().name() << std::endl;
    throw "Error unknowen type";
  }
}

///is the main antrypoint in to the code generation
std::any TopLevelVisitor::visitFile(SceneParser::FileContext *ctx){
  ///do the preperation
  ///create the function Table
  {
    funcs.emplace(
        MAIN_FUNC, 
        Function(TURTEL_MAIN_FUNC_NAME, VarType::VOID, ctx->main())
        );
    for (auto i : ctx->pathdef()) {
      if(!funcs .emplace( i->ID()->getText(), 
                  Function( USR_FUNC_PREFIX + i->ID()->getText(),
                  VarType::VOID,
                  i)
                )
                .second){
        throw "duplicate function names"; //TODO better error
      }
    }
    for (auto i : ctx->calcdef()) {
      if(!funcs .emplace ( i->ID()->getText(), 
                  Function( USR_FUNC_PREFIX + i->ID()->getText(),
                  VarType::DOUBLE,
                  i)
                )
                .second){
        throw "duplicate function names"; //TODO better error
      }
    }
  }

  envVar = std::unordered_map<std::string, Variable>({
    {std::string(RND_NAME),Variable(VarType::RENDERER,"__env_rnd")},
    {std::string(WINDOW_X), Variable(VarType::CONST_DOUBLE, "__env_wx")},
    {std::string(WINDOW_Y), Variable(VarType::CONST_DOUBLE, "__env_wy")},
    {std::string(POS_X),Variable(VarType::DOUBLE,"__env_posX")},
    {std::string(POS_Y),Variable(VarType::DOUBLE,"__env_posY")},
    {std::string(MAX_X),Variable(VarType::CONST_DOUBLE,"__env_maxX")},
    {std::string(MAX_Y),Variable(VarType::CONST_DOUBLE,"__env_maxY")},
    {std::string(ROTATION),Variable(VarType::DOUBLE,"__env_rot")},
    {std::string(COLOR_R),Variable(VarType::DOUBLE,"__env_red")},
    {std::string(COLOR_G),Variable(VarType::DOUBLE,"__env_green")},
    {std::string(COLOR_B),Variable(VarType::DOUBLE,"__env_blue")},
    {std::string(TEXTURE_NAME), Variable(VarType::TESXTUR, "__env_textur")},
    {std::string(WINDOW_NAME), Variable(VarType::WINDOW, "__env_window")},
    {std::string(EVENT_NAME), Variable(VarType::EVENT, "__env_event")},
  });
  //TODO add the real env Vars!
  {
    auto i = GlobalVarVisitor().getVariableContext(ctx);
    envVar.insert(i.begin(), i.end());
  }
  ///start generation of the output code
  output
    ///add includes
    << "//standart includes\n"
    << "#include <SDL2/SDL.h>\n"
    << "#include <SDL2/SDL_image.h>\n"
    << "#include <math.h>\n"
    << "#include <stdio.h>\n"
    << "#include <stdlib.h>\n"
    << std::endl
    ///add struct definitions
    << "typedef struct {\n"
    << "  double posX;\n"
    << "  double posY;\n"
    << "  double rotation;\n"
    << "} Marker;\n"
    << std::endl
    ///add gloabal vars
    << "#define MARKER_STACK_CAPACITY 4096\n\n"
    << "static Marker markerStack[MARKER_STACK_CAPACITY];\n"
    << "static int markerStackTop = -1;\n"
    << envVar.at(WINDOW_X).getTypeAndName() << "=800;\n"
    << envVar.at(WINDOW_Y).getTypeAndName() << "=600;\n"
    << envVar.at(TEXTURE_NAME).getTypeAndName() << ";\n"
    << envVar.at(WINDOW_NAME).getTypeAndName() << ";\n"
    << envVar.at(EVENT_NAME).getTypeAndName() << ";\n"
    << std::endl
    ///function declarations
    << "//declaration of Turtel HelperFuncs\n"
    << "void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture);\n"
    << "void __envfunc_fin(const double ret, SDL_Renderer * rnd);\n" 
    << "void __envfunc_stop(const double ret, SDL_Renderer * rnd);\n"
    ;
  ///get all the usr function definitions
  for (auto i : funcs) {
    output << i.second.getFunctionDeclaration();
  }

  output 
    << std::endl
    ///add the main function
    << "intmain(int argc, const char *argv[]){\n"///<sdl init
    << "  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);\n"///<creat window
    ///define the window
    << "  "
    << envVar.at(WINDOW_NAME).getTypeAndName()
    << "= SDL_CreateWindow( \"Main Window\", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, "
    << envVar.at(WINDOW_X).getTypeAndName() << ',' 
    << envVar.at(WINDOW_Y).getTypeAndName() << ','
    << "SDL_WINDOW_SHOWN );\n"
    ///Define renderer
    << "  " 
    << envVar.at(RND_NAME).getTypeAndName() 
    << " = SDL_CreateRenderer("
    << envVar.at(WINDOW_NAME).getTypeAndName()
    << ", -1, SDL_RENDERER_ACCELERATED);\n"
    ///define the texture
    << "  " 
    << envVar.at(TEXTURE_NAME).getTypeAndName() 
    << " = SDL_CreateTexture( " 
    << envVar.at(RND_NAME).getTypeAndName() 
    << ", SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, " 
    << envVar.at(WINDOW_X).getTypeAndName() 
    << ", " 
    << envVar.at(WINDOW_Y).getTypeAndName() 
    << ");\n"
    ///define events
    << "  SDL_Event events;\n"
    //switch to correct backbuffer (internal textur)
    << GenPresent
    << funcs.at(MAIN_FUNC).getFunctionCall({envVar.at(RND_NAME)}) << ';'///<call Turtel main
    << "  __envfunc_fin(0, " << envVar.at(RND_NAME).getTypeAndName() << ");\n"///<call the end functions
  //main end
    << "}\n"
    << std::endl
    ///add env Functions
    << "void __envfunc_stop(const double ret, SDL_Renderer * rnd){\n"
#ifndef UNIT_TEST
    << "  do{\n"
#ifndef NDEBUG
    << "    printf(\"Event Loop\\n\");\n"
#endif
    << "    SDL_WaitEvent(&" << envVar.at(EVENT_NAME).getTypeAndName() << ");\n"
    << "    switch ("<< envVar.at(EVENT_NAME).getTypeAndName() << ".type){\n"
    << "      case SDL_KEYDOWN:\n"
    << "      case SDL_QUIT: goto SDL_DEINIT_LABLE;\n"
    << "      default: break;\n"
    << "    }\n"
    << "  }while(1);\n"
#endif
    << "SDL_DEINIT_LABLE:\n"
    << "  __envfunc_stop(ret, rnd);\n"
    << "}\n"
    << "void __envfunc_fin(const double ret, SDL_Renderer * rnd){\n" 
    << "  SDL_DestroyRenderer(rnd);\n"
    << "  SDL_DestroyWindow( " << envVar.at(WINDOW_NAME).getTypeAndName() << ");\n"
    << "  SDL_Quit();\n"
    << "  exit((int) ret);\n"
    << "}\n"
    << "void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture) {\n"
    << "  SDL_Texture* target = SDL_GetRenderTarget(renderer);\n"
    << "  SDL_SetRenderTarget(renderer, texture);\n"
    << "  int width, height;\n"
    << "  SDL_QueryTexture(texture, NULL, NULL, &width, &height);\n"
    << "  SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);\n"
    << "  SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);\n"
    << "  IMG_SavePNG(surface, file_name);\n"
    << "  SDL_FreeSurface(surface);\n"
    << "  SDL_SetRenderTarget(renderer, target);\n"
    << "}\n"
    << "void pushMarker(Marker m) {\n"
    << "  if (markerStackTop < MARKER_STACK_CAPACITY - 1) {\n"
    << "    markerStack[++markerStackTop] = m;\n"
    << "  } else {\n"
    << "    fprintf(stderr, \"Fehler: Marker-Stack überlaufen!\\n\");\n"
    << "    exit(EXIT_FAILURE);\n"
    << "  }\n"
    << "}\n"
    << "Marker popMarker(void) {\n"
    << "  if (markerStackTop >= 0) {\n"
    << "    return markerStack[markerStackTop--];\n"
    << "  }\n"
    << "  else {\n"
    << "    fprintf(stderr, \"Fehler: Marker-Stack leer!\\n\");\n"
    << "    exit(EXIT_FAILURE);\n"
    << "  }\n"
    << "}\n"
    << std::endl
    ;
  ///generate Turtel main
  ctx->main()->accept(this);
  output << std::endl;
  ///generate all the calcdefs
  for (auto i : ctx->calcdef()) {
    i->accept(this);
  }
  output << std::endl;
  ///generate all the pathdefs
  for (auto i : ctx->pathdef()) {
    i->accept(this);
  }
  output << std::endl;

  return std::any();
}

std::any TopLevelVisitor::visitMain(SceneParser::MainContext *ctx){
  output
    ///output the function header
    << funcs.at(MAIN_FUNC).Implement()
    ;
  ctxVar = VarVisitor().getVariableContext(ctx); 
  ///define all the Variables
  for (auto i : ctxVar) {
    output << i.second.getTypeAndName();
    bool isHeaderDefine = false;
    for (auto j : funcs.at(MAIN_FUNC).getHeaderVars()) {
      if(i.second.name == j.name){
        isHeaderDefine = true;
        output << " = " << j.getName() << " ;\n";
      }
    }
    if (!isHeaderDefine) output << " = 0;\n";
  }
  ///implement all the commands
  ctx->statList()->accept(this);
  ///add the closing bracket
  output << "}\n";
  return std::any();
} 
std::any TopLevelVisitor::visitCalcdef(SceneParser::CalcdefContext *ctx) {
  output
    ///output the function header
    << funcs.at(ctx->ID()->getText()).Implement()
    ;
  ctxVar = VarVisitor().getVariableContext(ctx); 
  ///define all the Variables
  for (auto i : ctxVar) {
    output << i.second.getTypeAndName();
    bool isHeaderDefine = false;
    for (auto j : funcs.at(ctx->ID()->getText()).getHeaderVars()) {
      if(i.second.name == j.name){
        isHeaderDefine = true;
        output << " = " << j.getName() << " ;\n";
      }
    }
    if (!isHeaderDefine) output << " = 0;\n";
  }
  ///implement all the commands
  ctx->statList()->accept(this);
  ///add the return statment
  output << "return " << UnwrapExpre(ctx->expr()) << ";\n";
  ///add the closing bracket
  output << "}\n";
  return std::any();
}
std::any TopLevelVisitor::visitPathdef(SceneParser::PathdefContext *ctx){
  output
    ///output the function header
    << funcs.at(ctx->ID()->getText()).Implement()
    ;
  ctxVar = VarVisitor().getVariableContext(ctx); 
  ///define all the Variables
  for (auto i : ctxVar) {
    output << i.second.getTypeAndName();
    bool isHeaderDefine = false;
    for (auto j : funcs.at(ctx->ID()->getText()).getHeaderVars()) {
      if(i.second.name == j.name){
        isHeaderDefine = true;
        output << " = " << j.getName() << " ;\n";
      }
    }
    if (!isHeaderDefine) output << " = 0;\n";
  }
  ///implement all the commands
  ctx->statList()->accept(this);
  ///add the closing bracket
  output << "}\n";
  return std::any();
} 

std::any TopLevelVisitor::visitFuncCall(SceneParser::FuncCallContext *ctx){
  std::string funcName = ctx->ID()->getText();
  if(!funcs.contains(funcName)){
    std::cout << "using function " << funcName << " which wasn't defined in the file" << std::endl;
    throw "Error"; //TODO;
  }
  return funcs.at(funcName).getFunctionCall(std::any_cast<std::vector<Variable>>(ctx->paramlist()->accept(this)));
}
std::any TopLevelVisitor::visitParamlist(SceneParser::ParamlistContext *ctx){
  return [&]() -> std::vector<Variable> {
  std::vector<Variable> ret;
  std::transform(ctx->var().begin(), ctx->var().end(),
                 std::back_inserter(ret),
                 [this](auto i) {
                   return envVar.at(std::any_cast<std::string>(i->accept(this)));
                 });
  return ret;
}();
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
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " < "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; ++"
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
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
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " > "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; --"
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
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
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " > "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; "
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
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
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " = "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[3]))
          << "; "
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
          << " < "
          << UnwrapExpre(dynamic_cast<SceneParser::ExprContext*>(ctx->children[5]))
          << "; "
          << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
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
///\return the corresponding Varible object or an error is thrown
std::any TopLevelVisitor::visitVariable(SceneParser::VariableContext *ctx){
  std::cerr << __func__ << std::endl;
  std::string nm = ctx->ID()->getText();
  if(ctxVar.contains(nm)) return ctxVar.at(nm);
  return envVar.at(nm);
}
///\return the corresponding Varible object or an error is thrown
std::any TopLevelVisitor::visitGlobalVariable(SceneParser::GlobalVariableContext *ctx){
  std::cerr << __func__ << std::endl;
  std::string nm = ctx->IncID()->getText();
  if(ctxVar.contains(nm)) return ctxVar.at(nm);
  return envVar.at(nm);
}
std::any TopLevelVisitor::visitStoreVar(SceneParser::StoreVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
    << " = "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}
std::any TopLevelVisitor::visitSubVar(SceneParser::SubVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
    << " -= "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}
std::any TopLevelVisitor::visitDivVar(SceneParser::DivVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
    << " /= "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}
std::any TopLevelVisitor::visitAddVar(SceneParser::AddVarContext *ctx){
  std::cerr << __func__ << std::endl;
  output
    << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
    << " += "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}
std::any TopLevelVisitor::visitMultVar(SceneParser::MultVarContext *ctx){
  output
    << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
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
    <<", " << envVar.at(RND_NAME).getName() << ");\n";
  return std::any();

}
std::any TopLevelVisitor::visitFinOK(SceneParser::FinOKContext *ctx){
  output 
    << "  __envfunc_fin(0, " << envVar.at(RND_NAME).getName() << ");\n";
  return std::any();
}
std::any TopLevelVisitor::visitStopOK(SceneParser::StopOKContext *ctx){
  output 
    << "  __envfunc_stop(0, " << envVar.at(RND_NAME).getName() << ");\n";
  return std::any();
}
std::any TopLevelVisitor::visitStopError(SceneParser::StopErrorContext *ctx){
  output 
    << "  __envfunc_stop("
    << UnwrapExpre(ctx->expr())
    <<", " << envVar.at(RND_NAME).getName() << ");\n";
  return std::any();
}
std::any TopLevelVisitor::visitClear(SceneParser::ClearContext *ctx){
  output  << "  SDL_RenderClear("
          << envVar.at(RND_NAME).getName()
          << ");\n"
          ;
  return std::any();
}
std::any TopLevelVisitor::visitDirection(SceneParser::DirectionContext *ctx){
  output  << "  " 
          << envVar.at(ROTATION).getName() 
          << " = ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}
std::any TopLevelVisitor::visitTurnRight(SceneParser::TurnRightContext *ctx){
  output  << "  " 
          << envVar.at(ROTATION).getName() 
          << " += ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}
std::any TopLevelVisitor::visitTurnLeft(SceneParser::TurnLeftContext *ctx){
  output  << "  " 
          << envVar.at(ROTATION).getName() 
          << " -= ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}
///go back to WINDOW_X/2 and WINDOW_Y which should be the middle of the bottom of the screen
std::any TopLevelVisitor::visitWaklHome(SceneParser::WaklHomeContext *ctx){
  output  << "  SDL_RenderDrawLine("
          << envVar.at(RND_NAME).getName() 
          << ", " 
          << envVar.at(POS_X).getName() 
          << ", "
          << envVar.at(POS_Y).getName() 
          << ", "
          << envVar.at(WINDOW_X).getName() << " / 2, "
          << envVar.at(WINDOW_Y).getName() 
          << ");\n"
          
          << envVar.at(POS_X).getName() 
          << " = " 
          << envVar.at(WINDOW_X).getName() 
          << " / 2;\n"

          << envVar.at(POS_Y).getName() 
          << " = " 
          << envVar.at(WINDOW_Y).getName() 
          << ";\n"
          ;

  return std::any();
}
///go back to WINDOW_X/2 and WINDOW_Y which should be the middle of the bottom of the screen
std::any TopLevelVisitor::visitJumpHome(SceneParser::JumpHomeContext *ctx){
  output  << envVar.at(POS_X).getName() 
          << " = " 
          << envVar.at(WINDOW_X).getName() 
          << " / 2;\n"

          << envVar.at(POS_Y).getName() 
          << " = " 
          << envVar.at(WINDOW_Y).getName() 
          << ";\n"
          ;

  return std::any();
}
///File local funtion to move POS_X and POS_Y
#define CalcPosX(len) \
  envVar.at(POS_X).getName() << " + " << (len) << " * cos(" << envVar.at(ROTATION).getName() << ')'
#define CalcPosY(len) \
  envVar.at(POS_X).getName() << " + " << (len) << " * sin(" << envVar.at(ROTATION).getName() << ')'
#define MovePositions(len) \
  envVar.at(POS_X).getName() << " = "\
  << CalcPosX((len))\
  << ";\n"\
  << "  " << envVar.at(POS_Y).getName() << " = "\
  << CalcPosY((len))\
  << ";\n"
/**
 *Expected output:
 *SDL_RenderDrawLine(@renderVar, @x, @y, @x + divx, @y + divy
 * */
std::any TopLevelVisitor::visitWalkFront(SceneParser::WalkFrontContext *ctx){
  output  
    << "  SDL_RenderDrawLine("
    << envVar.at(RND_NAME).getName() 
    << ", " 
    << envVar.at(POS_X).getName() 
    << ", "
    << envVar.at(POS_Y).getName() 
    << ", "
    << CalcPosX(UnwrapExpre(ctx->expr()))
    << ","
    << CalcPosY(UnwrapExpre(ctx->expr()))
    << ");\n"

    << MovePositions(UnwrapExpre(ctx->expr()))
    << GenPresent
    ;
  return std::any();
}
std::any TopLevelVisitor::visitWalkBack(SceneParser::WalkBackContext *ctx){
  output  
    << "  SDL_RenderDrawLine("
    << envVar.at(RND_NAME).getName() 
    << ", " 
    << envVar.at(POS_X).getName() 
    << ", "
    << envVar.at(POS_Y).getName() 
    << ", "
    << CalcPosX("-(" + UnwrapExpre(ctx->expr()) + ')')
    << ","
    << CalcPosY("-(" + UnwrapExpre(ctx->expr()) + ')')
    << ");\n"

    << MovePositions("-(" + UnwrapExpre(ctx->expr()) + ')')
    << GenPresent
    ;
  return std::any();
}
std::any TopLevelVisitor::visitJumpFront(SceneParser::JumpFrontContext *ctx){
  output << MovePositions(UnwrapExpre(ctx->expr()));
  return std::any();
}
std::any TopLevelVisitor::visitJumpBack(SceneParser::JumpBackContext *ctx){
  output << MovePositions("-(" + UnwrapExpre(ctx->expr()) + ')');
  return std::any();
}
std::any TopLevelVisitor::visitSave(SceneParser::SaveContext *ctx){
  output
    << "  save_texture(\"" 
      << ctx->ID()->getText() << ".png\", "
      << envVar.at(RND_NAME).getName() << ", "
      << envVar.at(TEXTURE_NAME).getName() 
    << ");\n"
    ;
  return  std::any();
}
std::any TopLevelVisitor::visitMark(SceneParser::MarkContext *ctx) {
  output << "  pushMarker((Marker){ "
         << envVar.at(POS_X).getName() << ", "
         << envVar.at(POS_Y).getName() << ", "
         << envVar.at(ROTATION).getName()
         << " });\n";
  return std::any();
}
std::any TopLevelVisitor::visitWalkMark(SceneParser::WalkMarkContext *ctx) {
  output << "  if (markerStackTop >= 0) {\n"
  	     << "      Marker m = popMarker();\n"
  	     << "      SDL_RenderDrawLine(" 
  	     << envVar.at(RND_NAME).getName() << ", "
  	     << envVar.at(POS_X).getName() << ", "
  	     << envVar.at(POS_Y).getName() << ", "
  	     << "m.posX, m.posY);\n"
  	     << "      " << envVar.at(POS_X).getName() << " = m.posX;\n"
  	     << "      " << envVar.at(POS_Y).getName() << " = m.posY;\n"
  	     << "      " << envVar.at(ROTATION).getName() << " = m.rotation;\n"
  	     << "  } else {\n"
  	     << "      fprintf(stderr, \"Fehler: Marker-Stack leer!\\n\");\n"
  	     << "      exit(EXIT_FAILURE);\n"
  	     << "  }\n";
  return std::any();
}
std::any TopLevelVisitor::visitJumpMark(SceneParser::JumpMarkContext *ctx) {
  output << "  if (markerStackTop >= 0) {\n"
         << "      Marker m = popMarker();\n"
         << "      " << envVar.at(POS_X).getName() << " = m.posX;\n"
         << "      " << envVar.at(POS_Y).getName() << " = m.posY;\n"
         << "      " << envVar.at(ROTATION).getName() << " = m.rotation;\n"
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
         << envVar.at(RND_NAME).getName() << ", "
         << rValue << ", "
         << gValue << ", "
         << bValue << ", 255);\n";
  
  output << "  " << envVar.at(COLOR_R).getName() << " = " << rValue << ";\n"
         << "  " << envVar.at(COLOR_G).getName() << " = " << gValue << ";\n"
         << "  " << envVar.at(COLOR_B).getName() << " = " << bValue << ";\n";

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
///\return a string or bool
std::any TopLevelVisitor::visitLesEqThan(SceneParser::LesEqThanContext *ctx){
  OperationMacro(<=);  
}
///\return a string or bool
std::any TopLevelVisitor::visitGreaterThan(SceneParser::GreaterThanContext *ctx){
  OperationMacro(>);  
}
///\return a string or bool
std::any TopLevelVisitor::visitLesThan(SceneParser::LesThanContext *ctx){
  OperationMacro(<);  
}
///\return a string or bool
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
///\return returns the string to acces the variable
std::any TopLevelVisitor::visitVarExpr(SceneParser::VarExprContext *ctx){
  return std::any_cast<Variable>(ctx->var()->accept(this)).getName();
}
