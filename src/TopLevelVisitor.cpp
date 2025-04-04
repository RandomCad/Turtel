#include "src/TopLevelVisitor.h"
#include "SceneParser.h"
#include "GlobalVarVisitor.h"
#include "VariableVisitor.h"
#include "src/InternalVarNames.h"
#include "src/LLVMInterface.h"
#include "src/Variable.h"

#include <any>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
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

TopLevelVisitor::TopLevelVisitor(const char * const fileName) 
  : llvm(LLVMInterface(fileName)), output(llvm.llvmFile) {
  output << std::setprecision( std::numeric_limits<int>::max() );
}

///define function to unpack expr return
std::string TopLevelVisitor::UnwrapExpre(SceneParser::ExprContext *ctx){
  std::any ret = ctx->accept(this);
  if(ret.type() == typeid(std::string))     return std::any_cast<std::string>(ret);
  else if (ret.type() == typeid(int64_t))   return std::to_string(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))    return std::to_string(std::any_cast<double>(ret));
  else if (ret.type() == typeid(Variable))  return std::any_cast<Variable>(ret).getName();
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

  ///add the static envirment Variables
  envVar = ENV_VAR;
  ///add the User Global varibales
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
    ;

  for (auto i : envVar){
    output << i.second.getDefinition();
  }

  output
    << std::endl
    ///function declarations
    << "//declaration of Turtel HelperFuncs\n"
    << "void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture);\n"
    << "void __envfunc_fin(const double ret);\n" 
    << "void __envfunc_stop(const double ret);\n"
    ;
  ///get all the usr function definitions
  for (auto i : funcs) {
    output << i.second.getFunctionDeclaration();
  }

  output 
    << std::endl
    ///add the main function
    << "int main(int argc, const char *argv[]){\n"///<sdl init
    ///init env global Vars
    << envVar.at(WINDOW_X).getName() << "=800;\n"
    << envVar.at(WINDOW_Y).getName() << "=600;\n" 
    << envVar.at(POS_X).getName() << '=' << envVar.at(WINDOW_X).getName() << "/2;\n"
    << envVar.at(POS_Y).getName() << '=' << envVar.at(WINDOW_Y).getName() << "/2;\n"
    << envVar.at(ROTATION).getName() << "=-M_PI/2;\n"
    << "SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);\n"///<creat window
    ///define the window
    << envVar.at(WINDOW_NAME).getName()
    << "= SDL_CreateWindow( \"Main Window\", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, "
    << envVar.at(WINDOW_X).getName() << ',' 
    << envVar.at(WINDOW_Y).getName() << ','
    << "SDL_WINDOW_SHOWN );\n"
    ///Define renderer
    << envVar.at(RND_NAME).getName() 
    << " = SDL_CreateRenderer("
    << envVar.at(WINDOW_NAME).getName()
    << ", -1, SDL_RENDERER_ACCELERATED);\n"
    ///Set the color of the renderer
    << "SDL_SetRenderDrawColor(" << envVar.at(RND_NAME).getName() << ",255,255,255,255);\n"
    ///define the texture
    << envVar.at(TEXTURE_NAME).getName() 
    << " = SDL_CreateTexture( " 
    << envVar.at(RND_NAME).getName() 
    << ", SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, " 
    << envVar.at(WINDOW_X).getName() 
    << ", " 
    << envVar.at(WINDOW_Y).getName() 
    << ");\n"
    //pars the cli args
    << envVar.at(ARGC).getName() << "=argc-1;\n"
    << envVar.at(ARGV).getName() << "=malloc(sizeof(double) * (argc-1));\n"
    << "for (const char **i =argv + 1; *i; ++i)" << envVar.at(ARGV).getName() << "[i-argv-1]=strtod(*i,NULL);\n"
    //switch to correct backbuffer (internal textur)
    << GenPresent
    << funcs.at(MAIN_FUNC).getFunctionCall({}) << ';'///<call Turtel main
    << '\n'
    << "__envfunc_stop(0);\n"///<call the end functions
  //main end
    << "}\n"
    << std::endl
    ///add env Functions
    << "void __envfunc_stop(const double ret){\n"
#ifndef UNIT_TEST
    << "  do{\n"
#ifndef NDEBUG
    << "    printf(\"Event Loop\\n\");\n"
#endif
    << "    SDL_WaitEvent(&" << envVar.at(EVENT_NAME).getName() << ");\n"
    << "    switch ("<< envVar.at(EVENT_NAME).getName() << ".type){\n"
    << "      case SDL_KEYDOWN:\n"
    << "      case SDL_QUIT: __envfunc_stop(ret);\n"
    << "      default: break;\n"
    << "    }\n"
    << "  }while(1);\n"
#endif
    << "  __envfunc_fin(ret);\n"
    << "}\n"
    << "void __envfunc_fin(const double ret){\n" 
    << "  SDL_DestroyRenderer(" << envVar.at(RND_NAME).getName() <<" );\n"
    << "  SDL_DestroyWindow( " << envVar.at(WINDOW_NAME).getName() << ");\n"
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
    << "  if (IMG_SavePNG(surface, file_name)) printf(\"The picture could not be saved\");\n"
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

  llvm.CallLLVM();

  return std::any();
}

std::any TopLevelVisitor::visitMain(SceneParser::MainContext *ctx){
  output
    ///output the function header
    << funcs.at(MAIN_FUNC).Implement()
    << std::endl
    ;
  ctxVar = VarVisitor().getVariableContext(ctx, {}); 
  ///define all the Variables
  for (auto i : ctxVar) {
    output << i.second.getDefinition();
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
    << std::endl
    ;
  ctxVar = VarVisitor().getVariableContext(ctx,funcs.at(ctx->ID()->getText()).getHeaderVars()); 
  ///define all the Variables
  for (auto i : ctxVar) {
    output << i.second.getDefinition("0");
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
    << std::endl
    ;
  ctxVar = VarVisitor().getVariableContext(ctx, funcs.at(ctx->ID()->getText()).getHeaderVars()); 
  ///define all the Variables
  for (auto i : ctxVar) {
    output << i.second.getDefinition("0");
  }
  ///implement all the commands
  ctx->statList()->accept(this);
  ///add the closing bracket
  output << "}\n";
  return std::any();
} 

std::any TopLevelVisitor::visitFuncCall(SceneParser::FuncCallContext *ctx){
  std::cerr << __func__ << std::endl;
  std::string funcName = ctx->ID()->getText();
  if(!funcs.contains(funcName)){
    std::cout << "using function " << funcName << " which wasn't defined in the file" << std::endl;
    throw "Error"; //TODO;
  }
  std::string ret = funcs.at(funcName).getName() + '(';
  for (auto i : ctx->expr()) {
    ret += UnwrapExpre(i) + ',';
  }
  if(ctx->expr().size() > 0) ret[ret.size() - 1] = ')';
  else ret += ')';
  return ret;
}
std::any TopLevelVisitor::visitPathCall(SceneParser::PathCallContext *ctx){
  std::cerr << __func__ << std::endl;
  std::string funcName = ctx->ID()->getText();
  if(!funcs.contains(funcName)){
    std::cout << "using function " << funcName << " which wasn't defined in the file" << std::endl;
    throw "Error"; //TODO;
  }
  std::string ret;
  for (auto i : ctx->expr()) {
    ret += UnwrapExpre(i) + ',';
  }
  if(ctx->expr().size() > 0) ret[ret.size() - 1] = ')';
  else ret += ')';
  output << funcs.at(funcName).getName() << '(' << ret << ";\n";
  return std::any();
}
std::any TopLevelVisitor::visitParamlist(SceneParser::ParamlistContext *ctx){
  std::vector<Variable> ret(ctx->var().size());
  for(size_t i = 0; i < ctx->var().size(); ++i){
    ret[i] = std::any_cast<Variable>(ctx->var()[i]->accept(this));
  }
  assert(cnt == ctx->var().size());
  return ret;
}

std::any TopLevelVisitor::visitIf(SceneParser::IfContext *ctx){
  std::any ret = ctx->cond()->accept(this);
  if(ret.type() == typeid(bool)){
    if(!std::any_cast<bool>(ret)){
      std::cerr << "if was constfolded to " << false << std::endl;
      if(ctx->else_()){
        std::cerr << "else branch is outputted" << std::endl;
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
      std::cerr << "if was constfolded to" << true << std::endl;
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
  if (ret.type() == typeid(int64_t))            output  << "#pragma unroll\n"
                                                        << "  for (size_t i = 0; i <" << std::any_cast<int64_t>(ret) << " ; ++i){\n";
  else if (ret. type() == typeid(std::string))  output  << "  for (size_t i = 0; i <" << std::any_cast<std::string>(ret) << " ; ++i){\n";
  else if (ret. type() == typeid(Variable))  output  << "  for (size_t i = 0; i <" << std::any_cast<Variable>(ret).getName() << " ; ++i){\n";
  else{
    throw "TODO"; //TODO;
  }
 for (auto i : ctx->stat()) {
    i->accept(this);
  }
  output  << "}\n";
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
///@return the corresponding Varible object or an error is thrown
///@throw out_of_range exception if the variable is unknowen this should be imposible except for test cases and is an indication of a mayor Programming error
std::any TopLevelVisitor::visitVariable(SceneParser::VariableContext *ctx){
  std::cerr << __func__ << std::endl;
  std::string nm = ctx->ID()->getText();
  if(ctxVar.contains(nm)) return ctxVar.at(nm);
  if(envVar.contains(nm)) return envVar.at(nm);
  std::cerr << "Unknown variable named: " << nm << " found at: " << std::endl;
  throw std::out_of_range("Var dosn't exist");
}
std::any TopLevelVisitor::visitPiVar(SceneParser::PiVarContext *ctx){
  return (double)M_PI;
}
std::any TopLevelVisitor::visitCLI(SceneParser::CLIContext *ctx){
  std::string num = ctx->CliID()->getText();
  num[0] = '0';
  size_t number = std::strtoull(num.c_str(),NULL,10);
  std::string ret = "(";
  ret += envVar.at(ARGC).getName();
  ret += ">";
  ret += std::to_string(number);
  ret += "?";
  ret += envVar.at(ARGV).getName();
  ret += "[";
  ret += std::to_string(number);
  ret += "]:0)";
  return ret;
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
    <<");\n";
  return std::any();

}
std::any TopLevelVisitor::visitFinOK(SceneParser::FinOKContext *ctx){
  output 
    << "  __envfunc_fin(0);\n";
  return std::any();
}
std::any TopLevelVisitor::visitStopOK(SceneParser::StopOKContext *ctx){
  output 
    << "  __envfunc_stop(0);\n";
  return std::any();
}
std::any TopLevelVisitor::visitStopError(SceneParser::StopErrorContext *ctx){
  output 
    << "  __envfunc_stop("
    << UnwrapExpre(ctx->expr())
    << ");\n";
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
std::any TopLevelVisitor::visitWalkHome(SceneParser::WalkHomeContext *ctx){
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
  envVar.at(POS_Y).getName() << " + " << (len) << " * sin(" << envVar.at(ROTATION).getName() << ')'
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
      << "./" << ctx->ID()->getText() << ".png\", "
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
  if(left.type()      == typeid(std::string)  && reigth.type() == typeid(std::string)) return std::any_cast<std::string>(left) + #symbol + std::any_cast<std::string>(reigth);\
  else if(left.type() == typeid(std::string)  && reigth.type() == typeid(int64_t)) return std::any_cast<std::string>(left) + #symbol + std::to_string(std::any_cast<int64_t>(reigth));\
  else if(left.type() == typeid(std::string)  && reigth.type() == typeid(double)) return std::any_cast<std::string>(left) + #symbol + std::to_string(std::any_cast<double>(reigth));\
  else if(left.type() == typeid(std::string)  && reigth.type() == typeid(Variable)) return std::any_cast<std::string>(left) + #symbol + std::any_cast<Variable>(reigth).getName();\
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(std::string)) return std::to_string(std::any_cast<int64_t>(left)) + #symbol + std::any_cast<std::string>(reigth);\
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(int64_t)) return std::any_cast<int64_t>(left) symbol std::any_cast<int64_t>(reigth);\
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(double)) return std::any_cast<int64_t>(left) symbol std::any_cast<double>(reigth);\
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(Variable)) return std::to_string(std::any_cast<int64_t>(left)) + #symbol + std::any_cast<Variable>(reigth).getName();\
  else if(left.type() == typeid(double)       && reigth.type() == typeid(std::string)) return std::to_string(std::any_cast<double>(left)) + #symbol + std::any_cast<std::string>(reigth);\
  else if(left.type() == typeid(double)       && reigth.type() == typeid(int64_t)) return std::any_cast<double>(left) symbol std::any_cast<int64_t>(reigth);\
  else if(left.type() == typeid(double)       && reigth.type() == typeid(double)) return std::any_cast<double>(left) symbol std::any_cast<double>(reigth);\
  else if(left.type() == typeid(double)       && reigth.type() == typeid(Variable)) return std::to_string(std::any_cast<double>(left)) + #symbol + std::any_cast<Variable>(reigth).getName();\
  else if(left.type() == typeid(Variable)     && reigth.type() == typeid(int64_t)) return std::any_cast<Variable>(left).getName() + #symbol + std::to_string(std::any_cast<int64_t>(reigth));\
  else if(left.type() == typeid(Variable)     && reigth.type() == typeid(double)) return std::any_cast<Variable>(left).getName() + #symbol + std::to_string(std::any_cast<double>(reigth));\
  else if(left.type() == typeid(Variable)     && reigth.type() == typeid(std::string)) return std::any_cast<Variable>(left).getName() + #symbol + std::any_cast<std::string>(reigth);\
  else if(left.type() == typeid(Variable)     && reigth.type() == typeid(Variable)) return std::any_cast<Variable>(left).getName() + #symbol + std::any_cast<Variable>(reigth).getName();\
  else{\
    throw std::runtime_error((std::string("todo:") + __func__ + ' ' + left.type().name() + " " + reigth.type().name()).c_str()); /*TODO:*/\
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
  if (ret.type() == typeid(int64_t))          return std::abs(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))      return std::abs(std::any_cast<double>(ret));
  else if(ret.type() == typeid(std::string))  return "fabs(" + std::any_cast<std::string>(ret) + ')';
  else if(ret.type() == typeid(Variable))     return "fabs(" + std::any_cast<Variable>(ret).getName() + ')';
  else
    throw std::runtime_error("todo:"); //TODO:
}
std::any TopLevelVisitor::visitNegate(SceneParser::NegateContext *ctx){
  assert(ctx->children.size() == 2);
  std::any number = ctx->children[1]->accept(this);
  if (number.type() == typeid(int64_t))           return -std::any_cast<int64_t>(number);
  else if (number.type() == typeid(double))       return -std::any_cast<double>(number);
  else if (number.type() == typeid(std::string))  return "-" + std::any_cast<std::string>(number);
  else if(number.type() == typeid(Variable))      return "-" + std::any_cast<Variable>(number).getName() ;
  else{
    throw std::runtime_error("coudn't cast number context to number");
  }
}
std::any TopLevelVisitor::visitNumExpr(SceneParser::NumExprContext *ctx){
  std::cout << __func__ << std::endl;
  return ctx->number()->accept(this);
}
std::any TopLevelVisitor::visitExp(SceneParser::ExpContext *ctx){
  std::cout << __func__ << std::endl;
  std::any left = ctx->children[0]->accept(this);
  std::any reigth = ctx->children[2]->accept(this);

  if     (left.type() == typeid(std::string)  && reigth.type() == typeid(std::string))  return "pow(" + std::any_cast<std::string>(left) + ',' + std::any_cast<std::string>(reigth) + ')';
  else if(left.type() == typeid(std::string)  && reigth.type() == typeid(int64_t))      return "pow(" + std::any_cast<std::string>(left) + ',' + std::to_string(std::any_cast<int64_t>(reigth)) + ')';
  else if(left.type() == typeid(std::string)  && reigth.type() == typeid(double))       return "pow(" + std::any_cast<std::string>(left) + ',' + std::to_string(std::any_cast<double>(reigth)) + ')';
  else if(left.type() == typeid(std::string)  && reigth.type() == typeid(Variable))     return "pow(" + std::any_cast<std::string>(left) + ',' + std::any_cast<Variable>(reigth).getName() + ')';
  if     (left.type() == typeid(Variable)  && reigth.type() == typeid(std::string))     return "pow(" + std::any_cast<Variable>(left).getName() + ',' + std::any_cast<std::string>(reigth) + ')';
  else if(left.type() == typeid(Variable)  && reigth.type() == typeid(int64_t))         return "pow(" + std::any_cast<Variable>(left).getName() + ',' + std::to_string(std::any_cast<int64_t>(reigth)) + ')';
  else if(left.type() == typeid(Variable)  && reigth.type() == typeid(double))          return "pow(" + std::any_cast<Variable>(left).getName() + ',' + std::to_string(std::any_cast<double>(reigth)) + ')';
  else if(left.type() == typeid(Variable)  && reigth.type() == typeid(Variable))        return "pow(" + std::any_cast<Variable>(left).getName() + ',' + std::any_cast<Variable>(reigth).getName() + ')';
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(std::string))  return "pow(" + std::to_string(std::any_cast<int64_t>(left)) + ',' + std::any_cast<std::string>(reigth) + ')';
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(Variable))     return "pow(" + std::to_string(std::any_cast<int64_t>(left)) + ',' + std::any_cast<Variable>(reigth).getName() + ')';
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(int64_t))      return std::pow(std::any_cast<int64_t>(left), std::any_cast<int64_t>(reigth));
  else if(left.type() == typeid(int64_t)      && reigth.type() == typeid(double))       return std::pow(std::any_cast<int64_t>(left), std::any_cast<double>(reigth));
  else if(left.type() == typeid(double)       && reigth.type() == typeid(std::string))  return "pow(" + std::to_string(std::any_cast<double>(left)) + '+' + std::any_cast<std::string>(reigth) + ')';
  else if(left.type() == typeid(double)       && reigth.type() == typeid(Variable))     return "pow(" + std::to_string(std::any_cast<double>(left)) + '+' + std::any_cast<Variable>(reigth).getName() + ')';
  else if(left.type() == typeid(double)       && reigth.type() == typeid(int64_t))      return std::pow(std::any_cast<double>(left), std::any_cast<int64_t>(reigth));
  else if(left.type() == typeid(double)       && reigth.type() == typeid(double))       return std::pow(std::any_cast<double>(left), std::any_cast<double>(reigth));
  else{
    throw std::runtime_error("todo:"); //TODO:
  }
}
std::any TopLevelVisitor::visitAdd(SceneParser::AddContext *ctx){
  OperationMacro(+);
}
std::any TopLevelVisitor::visitDim(SceneParser::DimContext *ctx){
  OperationMacro(-);
}
std::any TopLevelVisitor::visitDife(SceneParser::DifeContext *ctx){
  OperationMacro(/);
}
std::any TopLevelVisitor::visitMult(SceneParser::MultContext *ctx){
  OperationMacro(*)
}
std::any TopLevelVisitor::visitKlamKon(SceneParser::KlamKonContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  if      (ret.type() == typeid(int64_t))     return ret;
  else if (ret.type() == typeid(double))      return ret;
  else if (ret.type() == typeid(Variable))    return ret;
  else if (ret.type() == typeid(std::string)) return "(" + std::any_cast<std::string>(ret) + ")";
  else{
    throw "TODO: visit Kalm unknowen type";
  }
}
///\return returns the string to acces the variable
std::any TopLevelVisitor::visitVarExpr(SceneParser::VarExprContext *ctx){
  return ctx->var()->accept(this);
}
