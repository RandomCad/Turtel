#pragma once

#include "Variable.h"
#include <map>
#include <vector>

class VariableHeandler{
  private:
    static const std::map<std::string, Variable> enviornment; 
    std::vector<std::map<std::string, Variable>> contextStack;

  public:
    const std::string getVariableNameString(const std::string&);
    const std::string getVariableDefinition(const std::string&);
  
};
