#include "CodeGenerator.Helper.h"
#include "src/VariableHeandler.h"

/**
 * @brief Generates the code for presenting the rendered content.
 * 
 * This function sets the render target, clears the renderer, copies the texture to the renderer,
 * presents the renderer, and then sets the render target back to the texture.
 * 
 * @param var The VariableHeandler object containing variable names.
 * @param output The output stream to write the generated code to.
 */
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


