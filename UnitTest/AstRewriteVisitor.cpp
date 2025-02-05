#include "gtest/gtest.h"

#include "../src/AstRewriteVisitor.h"
#include "../libs/SceneLexer.h"
#include "../libs/SceneParser.h"

using  namespace antlr4;

TEST(RewriteAstVisitor, JumpBack){
  std::stringstream stream;
  stream 
    << "jump back 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.stat();
  {
    ASSERT_TRUE(astStart);
    ASSERT_EQ(astStart->children.size(), 1);
    auto zwi = dynamic_cast<SceneParser::JumpBackContext*>(astStart->children[0]);
    ASSERT_TRUE(zwi);
    ASSERT_EQ(typeid(zwi->expr()), typeid(SceneParser::ExprContext*));
    auto numExpr = dynamic_cast<SceneParser::NumExprContext*>(zwi->expr());
    ASSERT_TRUE(numExpr);
    ASSERT_TRUE(numExpr->number());
    ASSERT_EQ(numExpr->number()->getText(), "5");
  }

  {
    AstRewriteVisitor vis;
    astStart->accept(&vis);

    ASSERT_TRUE(astStart);
    ASSERT_EQ(astStart->children.size(), 1);
    auto zwi = dynamic_cast<SceneParser::JumpFrontContext*>(astStart->children[0]);
    ASSERT_TRUE(zwi);
    ASSERT_EQ(typeid(zwi->expr()), typeid(SceneParser::ExprContext*));
    auto numExpr = dynamic_cast<SceneParser::NegateContext*>(zwi->expr());
    ASSERT_TRUE(numExpr);
    ASSERT_TRUE(numExpr->children.size());
    ASSERT_EQ(1, numExpr->children.size());
    auto testZwi = (dynamic_cast<SceneParser::ExprContext*>(numExpr->children[0]));
    ASSERT_TRUE(testZwi);
    std::cout << testZwi->getText() << std::endl;
    std::cout << typeid(testZwi).name() << std::endl;
    ASSERT_TRUE(dynamic_cast<SceneParser::NumExprContext*>(numExpr->children[0]));
    //ASSERT_TRUE(numExpr->number());
  }
}

TEST(RewriteAstVisitor, WalkBack){
  std::stringstream stream;
  stream 
    << "walk back 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.stat();
  {
    ASSERT_TRUE(astStart);
    ASSERT_EQ(astStart->children.size(), 1);
    auto zwi = dynamic_cast<SceneParser::WalkBackContext*>(astStart->children[0]);
    ASSERT_TRUE(zwi);
    ASSERT_EQ(typeid(zwi->expr()), typeid(SceneParser::ExprContext*));
    auto numExpr = dynamic_cast<SceneParser::NumExprContext*>(zwi->expr());
    ASSERT_TRUE(numExpr);
    ASSERT_TRUE(numExpr->number());
    ASSERT_EQ(numExpr->number()->getText(), "5");
  }
  
  {
    AstRewriteVisitor vis;
    astStart->accept(&vis);

    ASSERT_TRUE(astStart);
    ASSERT_EQ(astStart->children.size(), 1);
    auto zwi = dynamic_cast<SceneParser::WalkFrontContext*>(astStart->children[0]);
    ASSERT_TRUE(zwi);
    ASSERT_EQ(typeid(zwi->expr()), typeid(SceneParser::ExprContext*));
    auto numExpr = dynamic_cast<SceneParser::NegateContext*>(zwi->expr());
    ASSERT_TRUE(numExpr);
    ASSERT_TRUE(numExpr->children.size());
    ASSERT_EQ(1, numExpr->children.size());
    auto testZwi = (dynamic_cast<SceneParser::ExprContext*>(numExpr->children[0]));
    ASSERT_TRUE(testZwi);
    std::cout << testZwi->getText() << std::endl;
    std::cout << typeid(testZwi).name() << std::endl;
    ASSERT_TRUE(dynamic_cast<SceneParser::NumExprContext*>(numExpr->children[0]));
    //ASSERT_TRUE(numExpr->number());
  }
}
