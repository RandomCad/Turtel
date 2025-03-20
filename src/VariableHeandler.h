#pragma once

#include "Variable.h"
#include <string>
#include <unordered_map>

class VariableHeandler{
  private:
    std::unordered_map<std::string, Variable> enviornment; 
    std::unordered_map<std::string, Variable> ctx;

  public:
    VariableHeandler(std::unordered_map<std::string, Variable> &&);
    VariableHeandler() {}
    const std::string getVariableNameString(const std::string&);
    const std::string getVariableDefinition(const std::string&);
    Variable getVariable(const std::string&);
    void setContext(const std::unordered_map<std::string, Variable> &);
  
};
