#include "Variable.h"
#include <string>

std::string Variable::getTypeAndName(){
  std::string ret;
  switch (this->type) {
    case RENDERER:
      ret += "SDL_Renderer * ";
      break;
  }
  return ret += name;
}
