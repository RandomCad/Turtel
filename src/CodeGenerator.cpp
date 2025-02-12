#include "CodeGenerator.h"
#include "../libs/SceneParser.h"
#include "InternalVarNames.h"
#include "CodeGenerator.h"
#include "CodeGenerator.Helper.h"
#include "AstRewriteVisitor.h"

#include <any>
#include <cmath>
#include <iostream>
#include <cstring>
#include <string>
#include <tree/ParseTreeType.h>


std::any CodeGenerator::visitMain(SceneParser::MainContext *ctx){
  std::cerr << "generating main body" << std::endl;
  output

    << "void TurtelMain(SDL_Renderer * " 
    << _variables.getVariableNameString(RND_NAME) 
    << "){\n"

    ///define and set the x Position to half the window size
    << "  " << _variables.getVariableDefinition(POS_X) 
    << '=' 
    << _variables.getVariableNameString(WINDOW_X) << "/2;\n"

    ///define and set the y Position to half the window size
    << "  " << _variables.getVariableDefinition(POS_Y) 
    << '=' 
    << _variables.getVariableNameString(WINDOW_Y) << ";\n"

    ///define and set the rotation to 0 the window size
    << "  " << _variables.getVariableDefinition(ROTATION) << "=270 * (M_PI/180);\n"
    ///define the color to be white
    << "  " << _variables.getVariableDefinition(COLOR_R) << "=100;\n"
    << "  " << _variables.getVariableDefinition(COLOR_G) << "=100;\n"
    << "  " << _variables.getVariableDefinition(COLOR_B) << "=100;\n"
    
    ///set the color for the Renderer
    << "  SDL_SetRenderDrawColor(" 
    << _variables.getVariableNameString(RND_NAME) << ','
    << _variables.getVariableNameString(COLOR_R) << ','
    << _variables.getVariableNameString(COLOR_G) << ','
    << _variables.getVariableNameString(COLOR_B) << ','
    << "255);\n"

    ///set the max positions for the turtel
    << "  " << _variables.getVariableDefinition(MAX_X) 
    << '=' 
    << _variables.getVariableNameString(WINDOW_X) << "/2;\n"
    << "  " << _variables.getVariableDefinition(MAX_Y) 
    << '=' 
    << _variables.getVariableNameString(WINDOW_Y) << "/2;\n"
    ;

  //visit all the contained statments(stat)
  for(auto i : ctx->stat()) i->accept(&_topVis);

  output
    << "}\n" 
    <<std::endl;
  return nullptr;
}
//formate of the C-File:
//1. includes
//2. functiondeclaration
//3. main -> setup of generaly neede things(like the SDL APP, seperat Stack, ...)
//4. TurtelMain -> start of the turtel Programm
//5. Turtel Funktions
void CodeGenerator::GenerateCode(){
  AddIncludes();
  AddGlobalVars();
  AddFunctionDeclaration();
  AddMain();
  astMain->accept(new AstRewriteVisitor());
  astMain->accept(this);
  AddTurtelFunctions();
}

void CodeGenerator::AddIncludes(){
  output 
    << "//standart includes\n"
    << "#include <SDL2/SDL.h>\n"
    << "#include <SDL2/SDL_image.h>\n"
    << "#include <math.h>\n"
    << std::endl;
  //add potential further includes
}

void CodeGenerator::AddFunctionDeclaration(){
  output
    << "//declaration of the Turtel Main:\n"
    << "void TurtelMain(" << _variables.getVariableDefinition(RND_NAME) << ");\n" //TODO add needed parameters
    ;

  ///get all the function definitions
  _funcs.getFunctionDeclarations(output);

}

void CodeGenerator::AddGlobalVars(){
  output 
    << _variables.getVariableDefinition(WINDOW_X) << "=800;\n"
    << _variables.getVariableDefinition(WINDOW_Y) << "=600;\n"
    << _variables.getVariableDefinition(TEXTURE_NAME) << ";\n"
    << _variables.getVariableDefinition(WINDOW_NAME) << ";\n"
    ;
  ///Global programed _variables
}

void CodeGenerator::AddMain(){
  //main head
  output
    << "int main(int argc, const char *argv[]){\n"
  ///sdl init
    << "  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);\n"
  ///creat window
    << "  " 
    << _variables.getVariableNameString(WINDOW_NAME)
    << " = SDL_CreateWindow( \"Main Window\", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, "
    << _variables.getVariableNameString(WINDOW_X) << ',' 
    << _variables.getVariableNameString(WINDOW_Y) << ','
    << "SDL_WINDOW_SHOWN );\n"

  ///Define renderer
    << "  " 
    << _variables.getVariableDefinition(RND_NAME) 
    << " = SDL_CreateRenderer("
    << _variables.getVariableNameString(WINDOW_NAME)
    << ", -1, SDL_RENDERER_ACCELERATED);\n"

    << "  " << _variables.getVariableNameString(TEXTURE_NAME) << " = SDL_CreateTexture( " << _variables.getVariableNameString(RND_NAME) << ", SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, " << _variables.getVariableNameString(WINDOW_X) << ", " << _variables.getVariableNameString(WINDOW_Y) << ");\n"
    << "  SDL_Event events;\n"
  //allocate stack

  ;
  //switch to correct backbuffer (internal textur)
  GenPresent(_variables, output);
  
  output
  //call TurtelMain
    << "  TurtelMain(" << _variables.getVariableNameString(RND_NAME) << ");\n" //TODO add the parameters
  //Implicit wait
    #ifndef UNIT_TEST
    << "  do{\n"
    #ifdef NDEBUG
    << "    printf(\"Event Loop\\n\");\n"
    #endif
    << "    SDL_WaitEvent(&events);\n"
    << "    switch (events.type){\n"
    << "      case SDL_KEYDOWN:\n"
    << "      case SDL_QUIT: goto SDL_DEINIT_LABLE;\n"
    << "      default: break;\n"
    << "    }\n"
    << "  }while(1);\n"
    #endif
    << "  SDL_DEINIT_LABLE:\n"
    ;
  std::cerr << "generate endFunc Call" << std::endl;
  //sdl Deinit
  _funcs.getFunctionCall(
      END_FUNC,
      std::vector<Variable>
      {
        Variable()
        _variables.getVariable(RND_NAME)
      }
      );

}

void CodeGenerator::AddTurtelFunctions(){
  _funcs.ImplementFunctions(output); 
}

CodeGenerator::CodeGenerator(std::ostream &outStream)
  : output(outStream), astBase(nullptr), _topVis(output, _variables), _mathVis(_variables) {}

CodeGenerator::CodeGenerator(std::ostream &outStream, SceneParser::FileContext* AstBase)
  : output(outStream), astBase(AstBase), astMain(astBase->main()), 
    astCalcdef(astBase->calcdef()), astPathdef(astBase->pathdef()),
    _topVis(output, _variables), _mathVis(_variables){
}

void CodeGenerator::ProgrammBase(){
  //Headers
  output 
    << "//Standart includes\n"
    << "#include <stdlib.h>\n"
    << "#include <math.h>\n"
    << "#include <SDL/SDL.h>\n"
  //Prototype generation
    << "//Prototypes\n"
  //main
    << "//Main\n"
    << "int main(int argc, const char *argv[]){\n";
}

void CodeGenerator::EndeMain(){
  output 
    << "//End of Main\n"
    << "}\n//Implimentation start for funktions";
}
///file lokal
size_t GetUniquNumber(){
  static size_t num = 0;
  return num++;
}
