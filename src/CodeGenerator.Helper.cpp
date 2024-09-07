#include "CodeGenerator.h"
#include "libs/SceneParser.h"
#include <any>
#include <cstring>
#include <string>
#include <tree/ParseTreeType.h>

#define TURTLE_MAIN_FUNC_CALL "TurtelMain(" RENDERER_NAME " )"
#define TURTLE_MAIN_FUNC_DEF "void " "TurtelMain(SDL_Renderer * " RENDERER_NAME ")"

//formate of the C-File:
//1. includes
//2. functiondeclaration
//3. main -> setup of generaly neede things(like the SDL APP, seperat Stack, ...)
//4. TurtelMain -> start of the turtel Programm
//5. Turtel Funktions
void CodeGenerator::GenerateCode(){
  AddIncludes();
  AddFunctionDeclaration();
  AddMain();
  AddTurtelMain();
  AddTurtelFunctions();
}

void CodeGenerator::AddIncludes(){
  output 
    << "//standart includes\n"
    << "#include <SDL2/SDL.h>\n"
    << "#include <math.h>\n"
    << std::endl;
  //add potential further includes
}

void CodeGenerator::AddFunctionDeclaration(){
  output
    << "//declaration of the Turtel Main:\n"
    << TURTLE_MAIN_FUNC_DEF ";\n" //TODO add needed parameters
    << std::endl
  //add pathdef Functions:
    << "//declaration of the pathdefs\n"
    << std::endl
  //TODO do
  //add calcdef Functions:
    << "//declaration of the calcdefs\n"
    << std::endl
  //TODO do
    ;
}

void CodeGenerator::AddMain(){
  //main head
  output
    << "int main(int argc, const char *argv[]){\n"
  //sdl init
    << "  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);\n"
    << "  SDL_Window* window = SDL_CreateWindow( \"Main Window\", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN );\n"
    << "  SDL_Renderer *" RENDERER_NAME " = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);\n"
    << "  SDL_Event events;\n"
  //allocate stack

  //call TurtelMain
    << "  " TURTLE_MAIN_FUNC_CALL ";\n" //TODO add the parameters
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
  //sdl Deinit
    << "  SDL_DestroyRenderer(" RENDERER_NAME ");\n"
    << "  SDL_DestroyWindow(window);\n"
    << "  SDL_Quit();\n"
  //main end
    << "}\n"
    << std::endl;
}

void CodeGenerator::AddTurtelMain(){
  //Func def:
  astMain->accept(this);
}

void CodeGenerator::AddTurtelFunctions(){
  //TODO:
}
CodeGenerator::CodeGenerator(std::ostream &outStream)
  : output(outStream), astBase(nullptr) {}

CodeGenerator::CodeGenerator(std::ostream &outStream, SceneParser::FileContext* AstBase)
  : output(outStream), astBase(AstBase), astMain(astBase->main()), 
    astCalcdef(astBase->calcdef()), astPathdef(astBase->pathdef()) {
  //ProgrammBase();
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

size_t GetUniquNumber(){
  static size_t num = 0;
  return num++;
}
