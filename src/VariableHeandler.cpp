#include "Variable.h"
#include "VariableHeandler.h"

#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "InternalVarNames.h"

const std::map<std::string, Variable> VariableHeandler::enviornment = {
 {std::string(RND_NAME),Variable(VarType::RENDERER,"__env_rnd")},
 {std::string(POS_X),Variable(VarType::DOUBLE,"__env_posX")},
 {std::string(POS_Y),Variable(VarType::DOUBLE,"__env_posY")},
 {std::string(ROTATION),Variable(VarType::DOUBLE,"__env_rot")},
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
