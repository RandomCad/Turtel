#include "libs/SceneParser.h"
#include "libs/SceneVisitor.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/VariableHeandler.h"

#include <ParserRuleContext.h>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Function{
  protected:
    const VarType retType;
    const std::string name;
    std::unordered_map<std::string, Variable> vars;
    antlr4::ParserRuleContext *ctx;
  public:
    Function( const std::string nm
            , const std::unordered_map<std::string, Variable> var
            , VarType retT
            , antlr4::ParserRuleContext *CTX) 
      : retType(retT), name(nm), vars(var), ctx(CTX) {}
    std::string getFunctionDeclaration();
    std::string getFunctionCall(const std::vector<Variable> var);
    void Implement(SceneVisitor *vis);
};
