#include <any>
#include <cassert>
#include <tree/TerminalNode.h>
#include "AstRewriteVisitor.h"
#include "SceneLexer.h"
#include "SceneParser.h"

std::any AstRewriteVisitor::visitJumpBack(SceneParser::JumpBackContext *ctx) {
    // Get the expression to negate
    SceneParser::ExprContext *expr = ctx->expr();
    assert(expr);
    std::cerr << ctx << std::endl;

    // Create a new NegateContext and properly assign its parent
    SceneParser::NegateContext *invertExpr = new SceneParser::NegateContext(expr);
    invertExpr->addChild(expr);

    // Replace the original expression with the negated one
    assert(ctx->children.size() == 3);
    assert(dynamic_cast<SceneParser::ExprContext*>(ctx->children[2]));
    ctx->children[2] = invertExpr;

    // Ensure the replacement worked
    assert(ctx->children[2] == invertExpr);

    // Create a new JumpFrontContext to replace JumpBack
    SceneParser::JumpFrontContext *invertJump = new SceneParser::JumpFrontContext(ctx);
    invertJump->children = ctx->children;

    // Ensure correct parent-child relationships
    invertJump->parent = ctx->parent;
    assert(invertJump->children.size() == 3);
    assert(invertJump->children[2] == invertExpr);

    // Update the parent node to replace JumpBack with JumpFront
    if (ctx->parent) {
        for (size_t i = 0; i < ctx->parent->children.size(); ++i) {
            if (ctx->parent->children[i] == ctx) {
                ctx->parent->children[i] = invertJump;
                break;  // Exit loop once replaced
            }
        }
    }

    return invertJump->accept(this);
}

std::any AstRewriteVisitor::visitWalkBack(SceneParser::WalkBackContext *ctx){
  // Get the expression to negate
  SceneParser::ExprContext *expr = ctx->expr();
  assert(expr);
  std::cerr << ctx << std::endl;

  // Create a new NegateContext and properly assign its parent
  SceneParser::NegateContext *invertExpr = new SceneParser::NegateContext(expr);
  invertExpr->addChild(expr);

  // Replace the original expression with the negated one
  assert(ctx->children.size() == 3);
  assert(dynamic_cast<SceneParser::ExprContext*>(ctx->children[2]));
  ctx->children[2] = invertExpr;

  // Ensure the replacement worked
  assert(ctx->children[2] == invertExpr);

  // Create a new JumpFrontContext to replace JumpBack
  SceneParser::WalkFrontContext *invertJump = new SceneParser::WalkFrontContext(ctx);
  invertJump->children = ctx->children;

  // Ensure correct parent-child relationships
  invertJump->parent = ctx->parent;
  assert(invertJump->children.size() == 3);
  assert(invertJump->children[2] == invertExpr);

  // Update the parent node to replace JumpBack with JumpFront
  if (ctx->parent) {
    for (size_t i = 0; i < ctx->parent->children.size(); ++i) {
      if (ctx->parent->children[i] == ctx) {
        ctx->parent->children[i] = invertJump;
          break;  // Exit loop once replaced
      }
    }
  }

  return invertJump->accept(this);
}
