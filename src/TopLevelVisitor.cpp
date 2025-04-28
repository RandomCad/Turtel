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

/**
 * @brief Unwraps the expression and returns its string representation.
 * 
 * This function accepts an expression context, evaluates it, and returns its string representation.
 * 
 * @param ctx The expression context to unwrap.
 * @return The string representation of the expression.
 */
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

/**
 * @brief Visits the file context and generates the code.
 * 
 * This function is the main entry point for code generation. It prepares the function table,
 * adds static environment variables, user global variables, and starts generating the output code.
 * 
 * @param ctx The file context to visit.
 * @return An empty std::any object.
 */
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
    << "do{\n"
#ifndef NDEBUG
    << "    printf(\"Event Loop\\n\");\n"
#endif
    << "SDL_PollEvent(&" << envVar.at(EVENT_NAME).getName() << ");\n"
    << "switch ("<< envVar.at(EVENT_NAME).getName() << ".type){\n"
    << "case SDL_KEYDOWN:\n"
    << "case SDL_QUIT: __envfunc_fin(ret);\n"
    << "default: break;\n"
    << "}\n"
    << "}while(1);\n"
#endif
    << "__envfunc_fin(ret);\n"
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

/**
 * @brief Visits the main context and generates the main function code.
 * 
 * This function generates the main function code, defines all variables, and implements all commands.
 * 
 * @param ctx The main context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the calcdef context and generates the function code.
 * 
 * This function generates the function code for a calculation definition, defines all variables,
 * implements all commands, and adds the return statement.
 * 
 * @param ctx The calcdef context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the pathdef context and generates the function code.
 * 
 * This function generates the function code for a path definition, defines all variables,
 * and implements all commands.
 * 
 * @param ctx The pathdef context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the calc function call context and generates the calc function call code.
 * 
 * This function generates the calc function call code for a given calc function name and input variables.
 * 
 * @param ctx The function call context to visit.
 * @return The function call code as a string.
 */
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

/**
 * @brief Visits the path function call context and generates the path function call code.
 * 
 * This function generates the path function call code for a given path function name and input variables.
 * 
 * @param ctx The function call context to visit.
 * @return The function call code as a string.
 */
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
  return ret;
}

/**
 * @brief Visits the if context and generates the if statement code.
 * 
 * This function generates the if statement code, including the else branch if present.
 * 
 * @param ctx The if context to visit.
 * @return An empty std::any object.
 */
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
    output << "if(" << std::any_cast<std::string>(ret) << "){\n";
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
    output  << "}\n";
    return std::any();
  }
  else{
    throw "todo"; //TODO
  }
}

/**
 * @brief Visits the while context and generates the while loop code.
 * 
 * This function generates the while loop code, including handling of infinite loops.
 * 
 * @param ctx The while context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the do-until context and generates the do-until loop code.
 * 
 * This function generates the do-until loop code, including handling of infinite loops.
 * 
 * @param ctx The do-until context to visit.
 * @return An empty std::any object.
 */
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
              << " starting with character: "
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

/**
 * @brief Visits the to-for context and generates the for loop code.
 * 
 * This function generates the for loop code for a given range.
 * If the number of loops is known, unrolling hints are generated.
 * 
 * @param ctx The to-for context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the simple up-for context and generates the for loop code.
 * 
 * This function generates the for loop code for a simple incrementing range.
 * If the number of loops is known, unrolling hints are generated.
 * 
 * @param ctx The simple up-for context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the simple down-for context and generates the for loop code.
 * 
 * This function generates the for loop code for a simple decrementing range.
 * If the number of loops is known, unrolling hints are generated.
 * 
 * @param ctx The simple down-for context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the step down-for context and generates the for loop code.
 * 
 * This function generates the for loop code for a decrementing range with a step value.
 * If the number of loops is known, unrolling hints are generated.
 * 
 * @param ctx The step down-for context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the step up-for context and generates the for loop code.
 * 
 * This function generates the for loop code for an incrementing range with a step value.
 * If the number of loops is known, unrolling hints are generated.
 * 
 * @param ctx The step up-for context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the variable context and retrieves the variable.
 * 
 * This function retrieves the variable object for the given variable context.
 * 
 * @param ctx The variable context to visit.
 * @return The corresponding Variable object.
 * @throw std::out_of_range exception if the variable is unknown.
 */
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
std::any TopLevelVisitor::visitMaxX(SceneParser::MaxXContext *ctx){
  return std::string("(" + envVar.at(WINDOW_X).getName() + "/2)");
}
std::any TopLevelVisitor::visitMaxY(SceneParser::MaxYContext *ctx) {
  return std::string("(" + envVar.at(WINDOW_Y).getName() + "/2)");
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

/**
 * @brief Visits the store variable context and generates the store variable code.
 * 
 * This function generates the code to store a value in a variable.
 * 
 * @param ctx The store variable context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the subtract variable context and generates the subtract variable code.
 * 
 * This function generates the code to subtract a value from a variable.
 * 
 * @param ctx The subtract variable context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the divide variable context and generates the divide variable code.
 * 
 * This function generates the code to divide a variable by a value.
 * 
 * @param ctx The divide variable context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the add variable context and generates the add variable code.
 * 
 * This function generates the code to add a value to a variable.
 * 
 * @param ctx The add variable context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the multiply variable context and generates the multiply variable code.
 * 
 * This function generates the code to multiply a variable by a value.
 * 
 * @param ctx The multiply variable context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitMultVar(SceneParser::MultVarContext *ctx){
  output
    << std::any_cast<Variable>(ctx->var()->accept(this)).getName()
    << " *= "
    << UnwrapExpre(ctx->expr())
    << ";\n"
    ;
  return std::any();
}

/**
 * @brief Visits the finish error context and generates the finish error code.
 * 
 * This function generates the code to finish the program with an error code.
 * 
 * @param ctx The finish error context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitFinError(SceneParser::FinErrorContext *ctx){
  output 
    << "  __envfunc_fin("
    << UnwrapExpre(ctx->expr())
    <<");\n";
  return std::any();

}

/**
 * @brief Visits the finish OK context and generates the finish OK code.
 * 
 * This function generates the code to finish the program successfully.
 * 
 * @param ctx The finish OK context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitFinOK(SceneParser::FinOKContext *ctx){
  output 
    << "  __envfunc_fin(0);\n";
  return std::any();
}

/**
 * @brief Visits the stop OK context and generates the stop OK code.
 * 
 * This function generates the code to stop the program successfully.
 * 
 * @param ctx The stop OK context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitStopOK(SceneParser::StopOKContext *ctx){
  output 
    << "  __envfunc_stop(0);\n";
  return std::any();
}

/**
 * @brief Visits the stop error context and generates the stop error code.
 * 
 * This function generates the code to stop the program with an error code.
 * 
 * @param ctx The stop error context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitStopError(SceneParser::StopErrorContext *ctx){
  output 
    << "  __envfunc_stop("
    << UnwrapExpre(ctx->expr())
    << ");\n";
  return std::any();
}

/**
 * @brief Visits the clear context and generates the clear screen code.
 * 
 * This function generates the code to clear the screen.
 * 
 * @param ctx The clear context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitClear(SceneParser::ClearContext *ctx){
  output  << "  SDL_RenderClear("
          << envVar.at(RND_NAME).getName()
          << ");\n"
          ;
  return std::any();
}

/**
 * @brief Visits the direction context and generates the direction code.
 * 
 * This function generates the code to set the direction.
 * 
 * @param ctx The direction context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitDirection(SceneParser::DirectionContext *ctx){
  output  << "  " 
          << envVar.at(ROTATION).getName() 
          << " = ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}

/**
 * @brief Visits the turn right context and generates the turn right code.
 * 
 * This function generates the code to turn right.
 * 
 * @param ctx The turn right context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitTurnRight(SceneParser::TurnRightContext *ctx){
  output  << "  " 
          << envVar.at(ROTATION).getName() 
          << " += ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}

/**
 * @brief Visits the turn left context and generates the turn left code.
 * 
 * This function generates the code to turn left.
 * 
 * @param ctx The turn left context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitTurnLeft(SceneParser::TurnLeftContext *ctx){
  output  << "  " 
          << envVar.at(ROTATION).getName() 
          << " -= ("
          << UnwrapExpre(ctx->expr())
          << ") * (M_PI/180);\n"
          ;
  return std::any();
}

/**
 * @brief Visits the walk home context and generates the walk home code.
 * 
 * This function generates the code to walk home.
 * 
 * @param ctx The walk home context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitWalkHome(SceneParser::WalkHomeContext *ctx){
  output  << "SDL_RenderDrawLine("
          << envVar.at(RND_NAME).getName() 
          << ", " 
          << envVar.at(POS_X).getName() 
          << ", "
          << envVar.at(POS_Y).getName() 
          << ", "
          << envVar.at(WINDOW_X).getName() << " / 2, "
          << envVar.at(WINDOW_Y).getName() << " / 2"
          << ");\n"
          
          << envVar.at(POS_X).getName() 
          << " = " 
          << envVar.at(WINDOW_X).getName() 
          << " / 2;\n"

          << envVar.at(POS_Y).getName() 
          << " = " 
          << envVar.at(WINDOW_Y).getName() 
          << " / 2;\n"
          ;

  return std::any();
}

/**
 * @brief Visits the jump home context and generates the jump home code.
 * 
 * This function generates the code to jump home.
 * 
 * @param ctx The jump home context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitJumpHome(SceneParser::JumpHomeContext *ctx){
  output  << envVar.at(POS_X).getName() 
          << " = " 
          << envVar.at(WINDOW_X).getName() 
          << " / 2;\n"

          << envVar.at(POS_Y).getName() 
          << " = " 
          << envVar.at(WINDOW_Y).getName() 
          << " / 2;\n"
          ;

  return std::any();
}

#define CalcPosX(len) \
  envVar.at(POS_X).getName() << " + " << (len) << " * cos(" << envVar.at(ROTATION).getName() << ')'
#define CalcPosY(len) \
  envVar.at(POS_Y).getName() << " + " << (len) << " * sin(" << envVar.at(ROTATION).getName() << ')'
#define MovePositions(len) \
  envVar.at(POS_X).getName() << " = "\
  << CalcPosX((len))\
  << ";\n"\
  << envVar.at(POS_Y).getName() << " = "\
  << CalcPosY((len))\
  << ";\n"

/**
 * @brief Visits the walk front context and generates the walk front code.
 * 
 * This function generates the code to walk forward.
 * 
 * @param ctx The walk front context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitWalkFront(SceneParser::WalkFrontContext *ctx){
  output  
    << "SDL_RenderDrawLine("
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

/**
 * @brief Visits the walk back context and generates the walk back code.
 * 
 * This function generates the code to walk backward.
 * 
 * @param ctx The walk back context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitWalkBack(SceneParser::WalkBackContext *ctx){
  output  
    << "SDL_RenderDrawLine("
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

/**
 * @brief Visits the jump front context and generates the jump front code.
 * 
 * This function generates the code to jump forward.
 * 
 * @param ctx The jump front context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitJumpFront(SceneParser::JumpFrontContext *ctx){
  output << MovePositions(UnwrapExpre(ctx->expr()));
  return std::any();
}

/**
 * @brief Visits the jump back context and generates the jump back code.
 * 
 * This function generates the code to jump backward.
 * 
 * @param ctx The jump back context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitJumpBack(SceneParser::JumpBackContext *ctx){
  output << MovePositions("-(" + UnwrapExpre(ctx->expr()) + ')');
  return std::any();
}

/**
 * @brief Visits the save context and generates the save code.
 * 
 * This function generates the code to save the current state of the UI to a file.
 * 
 * @param ctx The save context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the mark context and generates the mark code.
 * 
 * This function generates the code to mark the current position.
 * 
 * @param ctx The mark context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitMark(SceneParser::MarkContext *ctx) {
  output << "  pushMarker((Marker){ "
         << envVar.at(POS_X).getName() << ", "
         << envVar.at(POS_Y).getName() << ", "
         << envVar.at(ROTATION).getName()
         << " });\n";
  return std::any();
}

/**
 * @brief Visits the walk mark context and generates the walk mark code.
 * 
 * This function generates the code to walk to the marked position.
 * 
 * @param ctx The walk mark context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the jump mark context and generates the jump mark code.
 * 
 * This function generates the code to jump to the marked position.
 * 
 * @param ctx The jump mark context to visit.
 * @return An empty std::any object.
 */
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

/**
 * @brief Visits the color command context and generates the color command code.
 * 
 * This function generates the code to set the drawing color.
 * 
 * @param ctx The color command context to visit.
 * @return An empty std::any object.
 */
std::any TopLevelVisitor::visitColorCmd(SceneParser::ColorCmdContext *ctx) {

  std::string rValue = UnwrapExpre(ctx->expr(0));
  std::string gValue = UnwrapExpre(ctx->expr(1));
  std::string bValue = UnwrapExpre(ctx->expr(2));

  output << "SDL_SetRenderDrawColor(" 
         << envVar.at(RND_NAME).getName() << ", "
         << rValue << ", "
         << gValue << ", "
         << bValue << ", 255);\n";
  
  output << envVar.at(COLOR_R).getName() << " = " << rValue << ";\n"
         << envVar.at(COLOR_G).getName() << " = " << gValue << ";\n"
         << envVar.at(COLOR_B).getName() << " = " << bValue << ";\n";

  return std::any();
}

/**
 * @brief Visits the integer context and returns its value.
 * 
 * This function visits an integer context and returns its value.
 * 
 * @param ctx The integer context to visit.
 * @return The integer value.
 */
std::any TopLevelVisitor::visitInt(SceneParser::IntContext *ctx){
  std::cerr << __func__ << std::endl;
  int64_t ret = std::stol(ctx->Num()->getSymbol()->getText());
  std::cerr << ret << std::endl;
  return ret;
}

/**
 * @brief Visits the float context and returns its value.
 * 
 * This function visits a float context and returns its value.
 * 
 * @param ctx The float context to visit.
 * @return The float value.
 */
std::any TopLevelVisitor::visitFloat(SceneParser::FloatContext *ctx){
  std::cerr << __func__ << std::endl;
  return std::stod(ctx->Float()->getSymbol()->getText());
}

/**
 * @brief Applies an operation between two operands.
 *
 * This macro retrieves the left and right operands from the parsing context and evaluates them
 * via their accept methods. It then checks the types of both operands (which can be std::string,
 * int64_t, double, or Variable) and performs an operation based on the provided symbol. Depending
 * on the operand types, it may concatenate strings, format numbers, or apply arithmetic operations.
 * If the operand types do not match any of the expected combinations, the macro throws a runtime error.
 *
 * @param symbol The operator symbol used in the operation.
 */
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

/**
 * @brief Visits the unequal context and generates the unequal comparison code.
 * 
 * This function generates the code for an unequal comparison.
 * 
 * @param ctx The unequal context to visit.
 * @return The result of the comparison.
 */
std::any TopLevelVisitor::visitUnequal(SceneParser::UnequalContext *ctx){
  OperationMacro(!=);  
}

/**
 * @brief Visits the equal context and generates the equal comparison code.
 * 
 * This function generates the code for an equal comparison.
 * 
 * @param ctx The equal context to visit.
 * @return The result of the comparison.
 */
std::any TopLevelVisitor::visitEqual(SceneParser::EqualContext *ctx){
  OperationMacro(==);  
}

/**
 * @brief Visits the less than or equal context and generates the comparison code.
 * 
 * This function generates the code for a less than or equal comparison.
 * 
 * @param ctx The less than or equal context to visit.
 * @return The result of the comparison.
 */
std::any TopLevelVisitor::visitLesEqThan(SceneParser::LesEqThanContext *ctx){
  OperationMacro(<=);  
}

/**
 * @brief Visits the greater than context and generates the comparison code.
 * 
 * This function generates the code for a greater than comparison.
 * 
 * @param ctx The greater than context to visit.
 * @return The result of the comparison.
 */
std::any TopLevelVisitor::visitGreaterThan(SceneParser::GreaterThanContext *ctx){
  OperationMacro(>);  
}

/**
 * @brief Visits the less than context and generates the comparison code.
 * 
 * This function generates the code for a less than comparison.
 * 
 * @param ctx The less than context to visit.
 * @return The result of the comparison.
 */
std::any TopLevelVisitor::visitLesThan(SceneParser::LesThanContext *ctx){
  OperationMacro(<);  
}

/**
 * @brief Visits the greater than or equal context and generates the comparison code.
 * 
 * This function generates the code for a greater than or equal comparison.
 * 
 * @param ctx The greater than or equal context to visit.
 * @return The result of the comparison.
 */
std::any TopLevelVisitor::visitGreaterEqThan(SceneParser::GreaterEqThanContext *ctx){
  OperationMacro(>=);  
}

/**
 * @brief Visits the clam condition context and generates the clam condition code.
 * 
 * This function generates the code for a clam condition.
 * 
 * @param ctx The clam condition context to visit.
 * @return The result of the condition.
 */
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

/**
 * @brief Visits the not condition context and generates the not condition code.
 * 
 * This function generates the code for a not condition.
 * 
 * @param ctx The not condition context to visit.
 * @return The result of the condition.
 */
std::any TopLevelVisitor::visitNotCond(SceneParser::NotCondContext *ctx){
  std::any ret = ctx->cond()->accept(this);
  if (ret.type() == typeid(bool))
    return !std::any_cast<bool>(ret);
  else if (ret.type() == typeid(std::string))
    return "!(" + std::any_cast<std::string>(ret) + ')';
  else
   throw "TODO"; //TODO:
}

/**
 * @brief Visits the and condition context and generates the and condition code.
 * 
 * This function generates the code for an and condition.
 * 
 * @param ctx The and condition context to visit.
 * @return The result of the condition.
 */
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

/**
 * @brief Visits the or condition context and generates the or condition code.
 * 
 * This function generates the code for an or condition.
 * 
 * @param ctx The or condition context to visit.
 * @return The result of the condition.
 */
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

/**
 * @brief Visits the absolute value context and returns the absolute value.
 * 
 * This function visits an absolute value context and returns the absolute value.
 * 
 * @param ctx The absolute value context to visit.
 * @return The absolute value.
 */
std::any TopLevelVisitor::visitABS(SceneParser::ABSContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  if (ret.type() == typeid(int64_t))          return std::abs(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))      return std::abs(std::any_cast<double>(ret));
  else if(ret.type() == typeid(std::string))  return "fabs(" + std::any_cast<std::string>(ret) + ')';
  else if(ret.type() == typeid(Variable))     return "fabs(" + std::any_cast<Variable>(ret).getName() + ')';
  else
    throw std::runtime_error("todo:"); //TODO:
}

/**
 * @brief Negates a given value.
 * Supports int64_t, double, and std::string.
 * 
 * @param ctx Context for the negate expression.
 * @return Negated value as std::any.
 * @throws std::runtime_error if the type is not supported.
 */
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

/**
 * @brief Visits a numeric expression and returns its value.
 * 
 * @param ctx Context for the numeric expression.
 * @return Evaluated number as std::any.
 */
std::any TopLevelVisitor::visitNumExpr(SceneParser::NumExprContext *ctx){
  std::cout << __func__ << std::endl;
  return ctx->number()->accept(this);
}

/**
 * @brief Performs exponentiation (power) on two values.
 * Supports numeric and string representations.
 * 
 * @param ctx Context for the exponentiation expression.
 * @return Result of the power operation as std::any.
 * @throws std::runtime_error if the type combination is not supported.
 */
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

/**
 * @brief Adds two values together.
 * Handles numbers and strings with various combinations.
 * 
 * @param ctx Context for the addition expression.
 * @return Result of the addition as std::any.
 * @throws std::runtime_error if the type combination is not supported.
 */
std::any TopLevelVisitor::visitAdd(SceneParser::AddContext *ctx){
  OperationMacro(+);
}

/**
 * @brief Handles subtraction operations.
 *
 * This function processes subtraction expressions by applying the '-' operator
 * to the operands provided in the context.
 *
 * @param ctx Pointer to the context containing the subtraction expression.
 * @return The result of the subtraction operation.
 */
std::any TopLevelVisitor::visitDim(SceneParser::DimContext *ctx){
  OperationMacro(-);
}

/**
 * @brief Handles division operations.
 *
 * This function processes division expressions by applying the '/' operator
 * to the operands provided in the context.
 *
 * @param ctx Pointer to the context containing the division expression.
 * @return The result of the division operation.
 */
std::any TopLevelVisitor::visitDife(SceneParser::DifeContext *ctx){
  OperationMacro(/);
}

/**
 * @brief Handles multiplication operations.
 *
 * This function processes multiplication expressions by applying the '*' operator
 * to the operands provided in the context.
 *
 * @param ctx Pointer to the context containing the multiplication expression.
 * @return The result of the multiplication operation.
 */
std::any TopLevelVisitor::visitMult(SceneParser::MultContext *ctx){
  OperationMacro(*)
}

/**
 * @brief Handles expressions within parentheses.
 *
 * This function evaluates expressions enclosed in parentheses. If the result
 * is a string, it adds parentheses around it; otherwise, it returns the result
 * as is. Throws an exception if the result type is unknown.
 *
 * @param ctx Pointer to the context containing the parenthesized expression.
 * @return The evaluated result, with parentheses added if it's a string.
 * @throws std::runtime_error if the result type is unknown.
 */
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

std::any TopLevelVisitor::visitCosCall(SceneParser::CosCallContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  if      (ret.type() == typeid(int64_t))     return std::cos(std::any_cast<int64_t>(ret) * M_PI/180);
  else if (ret.type() == typeid(double))      return std::cos(std::any_cast<double>(ret) * M_PI/180);
  else if (ret.type() == typeid(Variable))    return "cos((" + std::any_cast<Variable>(ret).getName() + ") * M_PI/180)";
  else if (ret.type() == typeid(std::string)) return "cos((" + std::any_cast<std::string>(ret) + ") * M_PI/180)";
  else{
    throw "TODO: visit cos unknowen type";
  }
}
std::any TopLevelVisitor::visitSinCall(SceneParser::SinCallContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  if      (ret.type() == typeid(int64_t))     return std::sin(std::any_cast<int64_t>(ret) * M_PI/180);
  else if (ret.type() == typeid(double))      return std::sin(std::any_cast<double>(ret) * M_PI/180);
  else if (ret.type() == typeid(Variable))    return "sin((" + std::any_cast<Variable>(ret).getName() + ") * M_PI/180)";
  else if (ret.type() == typeid(std::string)) return "sin((" + std::any_cast<std::string>(ret) + ") * M_PI/180)";
  else{
    throw "TODO: visit cos unknowen type";
  }
}
std::any TopLevelVisitor::visitSqrtCall(SceneParser::SqrtCallContext *ctx){
  std::any ret = ctx->expr()->accept(this);
  if      (ret.type() == typeid(int64_t))     return std::sqrt(std::any_cast<int64_t>(ret));
  else if (ret.type() == typeid(double))      return std::sqrt(std::any_cast<double>(ret));
  else if (ret.type() == typeid(Variable))    return "sqrt(" + std::any_cast<Variable>(ret).getName() + ')';
  else if (ret.type() == typeid(std::string)) return "sqrt(" + std::any_cast<std::string>(ret) + ')';
  else{
    throw "TODO: visit cos unknowen type";
  }
}
std::any TopLevelVisitor::visitRandCall(SceneParser::RandCallContext *ctx){
  std::any ret1 = ctx->expr(0)->accept(this);
  std::any ret2 = ctx->expr(1)->accept(this);
  std::string p1, p2;
  
  if      (ret1.type() == typeid(int64_t))     p1 = std::to_string(std::any_cast<int64_t>(ret1));
  else if (ret1.type() == typeid(double))      p1 = std::to_string(std::any_cast<double>(ret1));
  else if (ret1.type() == typeid(Variable))    p1 = std::any_cast<Variable>(ret1).getName();
  else if (ret1.type() == typeid(std::string)) p1 = std::any_cast<std::string>(ret1) ;
  else{
    throw "TODO: visit cos unknowen type";
  }

  if      (ret2.type() == typeid(int64_t)){
    int64_t zwi = std::any_cast<int64_t>(ret2);
    if      (ret1.type() == typeid(int64_t))     p2 = std::to_string(std::any_cast<int64_t>(ret1) - zwi);
    else if (ret1.type() == typeid(double))      p2 = std::to_string(std::any_cast<double>(ret1)  - zwi);
    else if (ret1.type() == typeid(Variable))    p2 = std::any_cast<Variable>(ret1).getName() + '-' + std::to_string(zwi);
    else if (ret1.type() == typeid(std::string)) p2 = std::any_cast<std::string>(ret1) + '-' + std::to_string(zwi);
    else{
      throw "TODO: visit cos unknowen type";
    }
  }
  else if (ret2.type() == typeid(double)){
    double zwi = std::any_cast<double>(ret2);
    if      (ret1.type() == typeid(int64_t))     p2 = std::to_string(std::any_cast<int64_t>(ret1)  - zwi);
    else if (ret1.type() == typeid(double))      p2 = std::to_string(std::any_cast<double>(ret1)   - zwi);
    else if (ret1.type() == typeid(Variable))    p2 = std::any_cast<Variable>(ret1).getName() + '-' + std::to_string(zwi);
    else if (ret1.type() == typeid(std::string)) p2 = std::any_cast<std::string>(ret1) + '-' + std::to_string(zwi);
    else{
      throw "TODO: visit cos unknowen type";
    }
  }
  else if (ret2.type() == typeid(Variable)){
    std::string zwi = std::any_cast<Variable>(ret2).getName();
    if      (ret1.type() == typeid(int64_t))     p2 = std::to_string(std::any_cast<int64_t>(ret1))  + '-' + zwi;
    else if (ret1.type() == typeid(double))      p2 = std::to_string(std::any_cast<double>(ret1))   + '-' + zwi;
    else if (ret1.type() == typeid(Variable))    p2 = std::any_cast<Variable>(ret1).getName()       + '-' + zwi;
    else if (ret1.type() == typeid(std::string)) p2 = std::any_cast<std::string>(ret1)              + '-' + zwi;
    else{
      throw "TODO: visit cos unknowen type";
    }
  }
  else if (ret2.type() == typeid(std::string)) {
    std::string zwi = std::any_cast<std::string>(ret2);
    if      (ret1.type() == typeid(int64_t))     p2 = std::to_string(std::any_cast<int64_t>(ret1))  + '-' + zwi;
    else if (ret1.type() == typeid(double))      p2 = std::to_string(std::any_cast<double>(ret1))   + '-' + zwi;
    else if (ret1.type() == typeid(Variable))    p2 = std::any_cast<Variable>(ret1).getName()       + '-' + zwi;
    else if (ret1.type() == typeid(std::string)) p2 = std::any_cast<std::string>(ret1)              + '-' + zwi;
    else{
      throw "TODO: visit cos unknowen type";
    }
  }
  else{
    throw "TODO: visit cos unknowen type";
  }
  
  return '(' + p1 +" + rand() % " + p2 + ')';
}

/**
 * @brief Returns the name of a variable.
 * 
 * @param ctx Context for the variable expression.
 * @return Variable name as std::any.
 */
std::any TopLevelVisitor::visitVarExpr(SceneParser::VarExprContext *ctx){
  return ctx->var()->accept(this);
}
