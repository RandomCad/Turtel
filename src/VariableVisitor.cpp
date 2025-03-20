#include "VariableVisitor.h"
#include "src/Variable.h"
#include <unordered_map>
std::any VarVisitor::visitVariable(SceneParser::VariableContext *ctx){
  ret[ctx->ID()->getText()] = Variable(VarType::DOUBLE, "__usr_" + ctx->ID()->getText());
  return std::any();
}


std::unordered_map<std::string, Variable> VarVisitor::getVariableContext(antlr4::ParserRuleContext *ctx){
  ret.clear();
  ctx->accept(this);
  return ret;
}
