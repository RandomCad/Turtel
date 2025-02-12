#include "src/Variable.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


class Function{
  protected:
    std::string name;
    std::vector<Variable> vars;
    VarType retType;
    Function(std::string nm, std::vector<Variable> var) 
      : name(nm),
        vars(var),
        retType(VarType::VOID)
        {} 
    Function(std::string nm, std::vector<Variable> var, VarType type) 
      : name(nm),
        vars(var),
        retType(type)
        {} 
  public:
    virtual std::string getFunctionDectaration();
    virtual std::string getFunctionCall(std::vector<Variable> input);
    virtual void Implement(std::ostream &out) = 0;
};

///definition of the names of the HelperFuncs
#define IMMAGE_FUNC_NAME "__env_immage_func"
#define END_FUNC_NAME "__env_end_func"
#define FIN_FUNC_NAME "__enc_fin_func"
class HelperFunc : public Function{
  static const std::unordered_map<std::string, int> funcNames;
  public:
    HelperFunc(std::string name, std::vector<Variable> var, VarType type) 
      : Function(name, var, type) {};
    void Implement(std::ostream &out) override;
};
