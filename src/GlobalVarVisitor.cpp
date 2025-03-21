#include "GlobalVarVisitor.h"
#include "src/Variable.h"
#include <unordered_map>

/**
 * @brief Visits a global variable context and adds the variable to the context.
 * 
 * This function visits a global variable context, creates a Variable object, and adds it to the context.
 * 
 * @param ctx The global variable context to visit.
 * @return An empty std::any object.
 */
std::any GlobalVarVisitor::visitGlobalVariable(SceneParser::GlobalVariableContext *ctx){
  ret[ctx->IncID()->getText()] = Variable(VarType::DOUBLE, "__usr_global_" + ctx->IncID()->getText());
  return std::any();
}

/**
 * @brief Retrieves the variable context.
 * 
 * This function clears the current variable context, visits the given parser rule context, and returns the updated variable context.
 * 
 * @param ctx The parser rule context to visit.
 * @return An unordered map of variable names to Variable objects.
 */
std::unordered_map<std::string, Variable> GlobalVarVisitor::getVariableContext(antlr4::ParserRuleContext *ctx){
  ret.clear();
  ctx->accept(this);
  return ret;
}
