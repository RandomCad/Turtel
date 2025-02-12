
#include "src/Function.h"
#include "src/Variable.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
class FuctionHandler{
  static std::unordered_map<std::string, std::shared_ptr<Function>> funcs;
  std::string getFunctionDeclaration(std::string funcName);
  public:
    std::string getFunctionCall(std::string funcName, std::vector<Variable> input);
    void ImplementFunctions(std::ostream &out);
    void getFunctionDeclarations(std::ostream &out);
};
