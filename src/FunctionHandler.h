
#include "SceneParser.h"
#include "SceneVisitor.h"
#include "src/Function.h"
#include "src/Variable.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
class FuctionHandler{
  std::unordered_map<std::string, Function> funcs;
  std::string getFunctionDeclaration(std::string funcName);
  public:
    FuctionHandler(SceneParser::MainContext*, 
                    std::vector<SceneParser::PathdefContext*>,
                    std::vector<SceneParser::CalcdefContext*>);
    std::string getFunctionCall(std::string funcName, std::vector<Variable> input);
    void ImplementFunctions(SceneVisitor *vis);
    void getFunctionDeclarations(std::ostream &out);
};
