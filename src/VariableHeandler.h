#pragma once

#include "Variable.h"
#include <map>
#include <unordered_map>
#include <vector>

class VariableHeandler{
  private:
    static const std::unordered_map<std::string, Variable> enviornment; 
    std::vector<std::map<std::string, Variable>> contextStack;

  public:
    const std::string getVariableNameString(const std::string&);
    const std::string getVariableDefinition(const std::string&);
    Variable getVariable(const std::string&);
  
};
