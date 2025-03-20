#include "Variable.h"
#include "VariableHeandler.h"
#include "InternalVarNames.h"
#include <string>
#include <unordered_map>

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

const std::string VariableHeandler::getVariableNameString(const std::string&nm){
  if(ctx.count(nm)){
    return ctx.at(nm).getName();
  }
  else if(enviornment.count(nm)){
    return enviornment.at(nm).getName();
  }
  //TODO error case
}

const std::string VariableHeandler::getVariableDefinition(const std::string&nm){
  if(ctx.count(nm)){
    return ctx.at(nm).getTypeAndName();
  }
  else if(enviornment.count(nm)){
    return enviornment.at(nm).getTypeAndName();
  }
  //TODO error case
}

Variable VariableHeandler::getVariable(const std::string&nm){
  if(ctx.count(nm)){
    return ctx.at(nm);
  }
  else if(enviornment.count(nm)){
    return enviornment.at(nm);
  }
  //TODO error case
}

void VariableHeandler::setContext(const std::unordered_map<std::string, Variable> &con){
  ctx = con;
}
