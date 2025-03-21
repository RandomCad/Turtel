#include "SceneBaseVisitor.h"
#include "SceneParser.h"
#include "src/Variable.h"
#include <ParserRuleContext.h>
#include <unordered_map>
#include <string>
class GlobalVarVisitor : public SceneBaseVisitor{
  std::unordered_map<std::string, Variable> ret;
  std::any visitGlobalVariable(SceneParser::GlobalVariableContext *ctx) override;
  public:
  std::unordered_map<std::string, Variable> getVariableContext(antlr4::ParserRuleContext *ctx);
};
