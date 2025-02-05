
#include "SceneBaseVisitor.h"
class AstRewriteVisitor : public SceneBaseVisitor {
  public:
    std::any visitJumpBack(SceneParser::JumpBackContext *ctx) override;
    std::any visitWalkBack(SceneParser::WalkBackContext *ctx) override;
};
