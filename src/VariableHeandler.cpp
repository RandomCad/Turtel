#include "Variable.h"
#include "VariableHeandler.h"
#include "InternalVarNames.h"
#include <string>
#include <unordered_map>

/**
 * @brief Constructor for VariableHeandler.
 *
 * Initializes the environment variables with default internal variables,
 * and adds the provided global variables to the environment.
 *
 * @param globals A map of global variables to be included in the environment.
 */
VariableHeandler::VariableHeandler(std::unordered_map<std::string, Variable> &&globals) :
  enviornment ({
    {std::string(RND_NAME),Variable(VarType::RENDERER,"__env_rnd")},
    {std::string(WINDOW_X), Variable(VarType::CONST_DOUBLE, "__env_wx")},
    {std::string(WINDOW_Y), Variable(VarType::CONST_DOUBLE, "__env_wy")},
    {std::string(POS_X),Variable(VarType::DOUBLE,"__env_posX")},
    {std::string(POS_Y),Variable(VarType::DOUBLE,"__env_posY")},
    {std::string(MAX_X),Variable(VarType::CONST_DOUBLE,"__env_maxX")},
    {std::string(MAX_Y),Variable(VarType::CONST_DOUBLE,"__env_maxY")},
    {std::string(ROTATION),Variable(VarType::DOUBLE,"__env_rot")},
    {std::string(COLOR_R),Variable(VarType::DOUBLE,"__env_red")},
    {std::string(COLOR_G),Variable(VarType::DOUBLE,"__env_green")},
    {std::string(COLOR_B),Variable(VarType::DOUBLE,"__env_blue")},
    {std::string(TEXTURE_NAME), Variable(VarType::TESXTUR, "__env_textur")},
    {std::string(WINDOW_NAME), Variable(VarType::WINDOW, "__env_window")},
    {std::string(EVENT_NAME), Variable(VarType::EVENT, "__env_event")},
  }){
    enviornment.insert(globals.begin(), globals.end());
}

/**
 * @brief Returns the internal name of a variable.
 *
 * Looks up the given variable name in the context or environment
 * and returns its internal name string.
 *
 * @param nm The name of the variable.
 * @return The internal name string of the variable.
 */
const std::string VariableHeandler::getVariableNameString(const std::string&nm){
  if(ctx.count(nm)){
    return ctx.at(nm).getName();
  }
  else if(enviornment.count(nm)){
    return enviornment.at(nm).getName();
  }
  //TODO error case
}

/**
 * @brief Returns the type and internal name of a variable.
 *
 * Searches the context and environment for the given variable name
 * and returns its declaration as a string (e.g. "double __name").
 *
 * @param nm The name of the variable.
 * @return A string representing the type and name.
 */
const std::string VariableHeandler::getVariableDefinition(const std::string&nm){
  if(ctx.count(nm)){
    return ctx.at(nm).getTypeAndName();
  }
  else if(enviornment.count(nm)){
    return enviornment.at(nm).getTypeAndName();
  }
  //TODO error case
}

/**
 * @brief Returns the full Variable object by name.
 *
 * Searches the context and environment for the given name
 * and returns the corresponding Variable object.
 *
 * @param nm The name of the variable.
 * @return The corresponding Variable object.
 */
Variable VariableHeandler::getVariable(const std::string&nm){
  if(ctx.count(nm)){
    return ctx.at(nm);
  }
  else if(enviornment.count(nm)){
    return enviornment.at(nm);
  }
  //TODO error case
}

/**
 * @brief Sets the context variables.
 *
 * Replaces the current context with the given variable map.
 *
 * @param con A map of variable names and corresponding Variable objects.
 */
void VariableHeandler::setContext(const std::unordered_map<std::string, Variable> &con){
  ctx = con;
}
