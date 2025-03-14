#include "GlobalVarVisitor.h"
#include "src/Variable.h"
#include <unordered_map>
std::any GlobalVarVisitor::visitGlobalVariable(SceneParser::GlobalVariableContext *ctx){
  ret[ctx->IncID()->getText()] = Variable(VarType::DOUBLE, "__usr_global_" + ctx->IncID()->getText());
  return std::any();
}


std::unordered_map<std::string, Variable> GlobalVarVisitor::getVariableContext(antlr4::ParserRuleContext *ctx){
  ret.clear();
  ctx->accept(this);
  return ret;
}
