#include "Variable.h"
#include "VariableHeandler.h"

#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "InternalVarNames.h"

const std::map<std::string, Variable> VariableHeandler::enviornment = {
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
};

const std::string VariableHeandler::getVariableNameString(const std::string&nm){
  for(std::vector<std::map<std::string,Variable>>::reverse_iterator i = contextStack.rbegin();i != contextStack.rend(); ++i){
    if(std::map<std::string,Variable>::iterator ret = i->find(nm); ret != i->end()){
      return ret->second.getName();
    }
  }
  if(auto ret = enviornment.find(nm); ret != enviornment.end()){
    return ret->second.getName();
  }
  //TODO error case
}

const std::string VariableHeandler::getVariableDefinition(const std::string&nm){
  for(auto i = contextStack.rbegin();i != contextStack.rend(); ++i){
    std::map<std::string,Variable>::iterator ret = i->find(nm);
    if( ret != i->end()){
      return ret->second.getTypeAndName();
    }
  }
  auto ret = enviornment.find(nm);
  if( ret != enviornment.end()){
    return ret->second.getTypeAndName();
  }
  //TODO error case
}
