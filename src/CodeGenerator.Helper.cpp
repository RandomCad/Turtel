#include "CodeGenerator.Helper.h"
#include "src/VariableHeandler.h"
void GenPresent(VariableHeandler &var, std::ostream & output){
  output
    << "  SDL_SetRenderTarget("
    << var.getVariableNameString(RND_NAME)
    << ", NULL);\n"
  
    << "    SDL_RenderClear( "
    << var.getVariableNameString(RND_NAME)
    << ");\n"
  
    << "  SDL_RenderCopy( "
    << var.getVariableNameString(RND_NAME)
    << ", "
    << var.getVariableNameString(TEXTURE_NAME)
    << ", NULL, NULL);\n"

    << "  SDL_RenderPresent( "
    << var.getVariableNameString(RND_NAME)
    << ");\n"

    << "  SDL_SetRenderTarget("
    << var.getVariableNameString(RND_NAME)
    << ", "
    << var.getVariableNameString(TEXTURE_NAME)
    << ");\n"
    ;
}


