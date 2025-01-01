#include "Variable.h"
#include <string>

std::string Variable::getTypeAndName() const{
  std::string ret;
  switch (this->type) {
    case RENDERER:
      ret += "SDL_Renderer * ";
      break;
    case CONST_DOUBLE:
      ret += "const ";
    case DOUBLE:
      ret += "double ";
      break;
  }
  return ret += name;
}
