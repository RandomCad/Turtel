
#include "SceneBaseVisitor.h"
#include "SceneParser.h"
#include "src/Variable.h"
#include <ParserRuleContext.h>
#include <unordered_map>
#include <string>
class VarVisitor : public SceneBaseVisitor{
  std::unordered_map<std::string, Variable> ret;
  std::any visitVariable(SceneParser::VariableContext *ctx) override;
  public:
  std::unordered_map<std::string, Variable> getVariableContext(antlr4::ParserRuleContext *,const std::vector<std::pair<std::string, Variable>> &);
};
