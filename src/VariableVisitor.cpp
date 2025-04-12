#include "VariableVisitor.h"
#include "src/Variable.h"
#include <unordered_map>

/**
 * @brief Visits a variable declaration in the parse tree.
 *
 * Adds a new variable of type DOUBLE to the internal context using the identifier
 * found in the parse tree. The variable name is prefixed with "__usr_".
 *
 * @param ctx Pointer to the parse context representing a variable.
 * @return An empty std::any object.
 */
std::any VarVisitor::visitVariable(SceneParser::VariableContext *ctx){
  ret.insert({ctx->ID()->getText(), Variable(VarType::DOUBLE, "__usr_" + ctx->ID()->getText())});
  return std::any();
}

/**
 * @brief Builds and returns a variable context from the parse tree and existing variables.
 *
 * Clears the internal variable context, adds any pre-existing variables, and then
 * traverses the parse tree to collect additional variable declarations.
 *
 * @param ctx The root context to start variable collection from.
 * @param exists A list of already known variables to include in the context.
 * @return A map containing all collected variables.
 */
std::unordered_map<std::string, Variable> VarVisitor::getVariableContext(
    antlr4::ParserRuleContext *ctx,
    const std::vector<std::pair<std::string, Variable>> &exists){
  ret.clear();
  for (auto i : exists) ret.insert(i);
  ctx->accept(this);
  return ret;
}
