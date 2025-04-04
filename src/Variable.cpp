#include "Variable.h"
#include <string>

/**
 * @brief Retrieves the type and name of the variable as a single string.
 *
 * Constructs a string that concatenates the variable's type and its name.
 *
 * @return A string representing the type and name of the variable.
 */
std::string Variable::getTypeAndName() const{
  std::string ret = VarTypeNS::getTypeName(this->type);
  return ret += name;
}

/**
 * @brief Provides the variable's declaration, optionally with initialization.
 *
 * If the variable is not yet defined, marks it as defined and returns a string
 * containing its declaration. For variables of type DOUBLE, the declaration includes
 * initialization with the provided assignment value.
 * If the variable is already defined, a comment is generated.
 *
 * @param assigne The value to assign to the variable upon declaration.
 * @return A string containing the variable's declaration, with initialization if applicable.
 */
const std::string Variable::getDefinition(const std::string assigne){
  if(isDefined) return "/*redifinition*/";
  isDefined = true;
  switch (type) {
    case DOUBLE: return getTypeAndName() + "=" + assigne + ";\n";
    default: return getTypeAndName() + ";\n";
  }
}

/**
 * @brief Converts a VarType enumeration value to its corresponding C++ type name.
 *
 * Maps a given VarType to its equivalent C++ type name as a string.
 *
 * @param type The VarType to be converted.
 * @return A string representing the corresponding C++ type name.
 */
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
