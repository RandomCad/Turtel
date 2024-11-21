#include "CodeGenerator.h"
#include "../libs/SceneParser.h"
#include <any>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
std::any CodeGenerator::visitMain(SceneParser::MainContext *ctx){
  output
    << "void " "TurtelMain(SDL_Renderer * " RENDERER_NAME "){\n"
    << "  " << _variables.getVariableDefinition("@x") << ";\n"
    << "  " << _variables.getVariableDefinition("@y") << ";\n";

  //visit all the contained statments(stat)
  for(auto i : ctx->children) i->accept(this);

  output
    << "}\n" 
    <<std::endl;
  return nullptr;
}


