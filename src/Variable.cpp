#include "Variable.h"
#include <string>

std::string Variable::getTypeAndName() const{
  std::string ret = VarTypeNS::getTypeName(this->type);
  return ret += name;
}

std::string VarTypeNS::getTypeName(VarType type){
  switch (type) {
    case CONST_CHAR_PTR:
      return "const char * ";
    case VOID:
      return "void ";
    case RENDERER:
      return "SDL_Renderer * ";
    case TESXTUR:
      return "SDL_Texture * ";
    case WINDOW:
      return "SDL_Window * ";
    case CONST_DOUBLE:
      return "const double ";
    case DOUBLE:
      return "double ";
    case EVENT:
      return "SDL_Event ";
  }
}
