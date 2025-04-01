#include "Variable.h"
#include <string>

std::string Variable::getTypeAndName() const{
  std::string ret = VarTypeNS::getTypeName(this->type);
  return ret += name;
}

const std::string Variable::getDefinition(const std::string assigne){
  if(isDefined) return "";
  isDefined = true;
  switch (type) {
    case DOUBLE: return getTypeAndName() + "=" + assigne + ";\n";
    default: return getTypeAndName() + ";\n";
  }
}

std::string VarTypeNS::getTypeName(VarType type){
  switch (type) {
    case CONST_CHAR_PTR:return "const char * ";
    case VOID:          return "void ";
    case RENDERER:      return "SDL_Renderer * ";
    case TESXTUR:       return "SDL_Texture * ";
    case WINDOW:        return "SDL_Window * ";
    case CONST_DOUBLE:  return "const double ";
    case DOUBLE:        return "double ";
    case EVENT:         return "SDL_Event ";
    case SIZE_T:        return "size_t ";
    case DOUBLE_ARR:    return "double *";
  }
}
