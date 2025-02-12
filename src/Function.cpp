#include "src/Function.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/VariableHeandler.h"
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, int> HelperFunc::funcNames = {
  {IMMAGE_FUNC_NAME, 0},
  {END_FUNC_NAME, 1},
  {FIN_FUNC_NAME, 2},
};

///returns the function declaration for this function
std::string Function::getFunctionDectaration(){
  //first add the returntype
  std::string ret = VarTypeNS::getTypeName(this->retType);
  //no the name and the opening '('
  ret += ' ' +  name + "(";
  //add all the variables
  for (Variable i : vars) {
    ret += i.getTypeAndName() + ", ";
  }
  //add the ')' and the ';'
  ret[ret.size()-2] = ')';
  ret[ret.size()-1] = ';';
  //add a '\n' and return the result
  return ret + '\n';
}

std::string Function::getFunctionCall(std::vector<Variable> input){
#ifndef NDEBUG
std::cerr << __func__ << std::endl;
#endif  
  if(input.size() != vars.size()){
    std::cerr << "input length unequal to function length" << std::endl;
    throw "input length unequal function length";
  }
  //no the name and the opening '('
  std::string ret = name + "(";
  //add all the variables
  for (size_t i=0; i < vars.size(); ++i) {
    if(vars[i].type != input[i].type){
      std::cerr << "unequal type " << i << std::endl << vars[i].type << "!=" << input[i].type << std::endl;
      throw "types arn't equal"; //TODO bool == const bool but const bool != bool
    }
    ret += input[i].name + ", ";
  }
  //add the ')' and the ';'
  ret[ret.size()-2] = ')';
  ret[ret.size()-1] = ' ';
  //add a '\n' and return the result
  return ret;
}

void HelperFunc::Implement(std::ostream &out){
  std::cerr << name << std::endl;
  switch (funcNames.at(this->name)) {
    case 0:
       out
        << VarTypeNS::getTypeName(this->retType) 
        << name 
        << "(" 
        << vars[0].getTypeAndName()
        << ", "
        << vars[1].getTypeAndName()
        << ", " 
        << vars[2].getTypeAndName()
        << "){\n"

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
        ;
      break;
    case 1:
      out
        << VarTypeNS::getTypeName(this->retType) 
        << name 
        << "(" 
        << vars[0].getTypeAndName()
        << ", "
        << vars[1].getTypeAndName()
        << "){\n"

        << "  SDL_DestroyRenderer(" 
        << vars[1].name 
        << ");\n"

        << "  SDL_DestroyWindow(" 
        << VariableHeandler().getVariableNameString(WINDOW_NAME) 
        << ");\n"
        
        << "  SDL_Quit();\n"

        << "  exit("
        << vars[0].name
        << ");\n"
        << "}\n"
        ;
      break;
    default:
      std::cerr << "not impl" << std::endl;
      throw "Not implemented";
  }
  return;
}
