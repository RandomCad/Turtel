#include <ANTLRInputStream.h>
#include <gtest/gtest.h>
#include <string>

#include "gtest/gtest.h"
#include "libs/SceneParser.h"
#include "libs/SceneLexer.h"
#include "src/VariableVisitor.h"

using  namespace antlr4;

TEST(VariableVisitor, GetVars){
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  walk _step\n"
    << "  store 5 in _step\n"
    << "  add _step to _wal5\n"
    << "end\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.main();

  ASSERT_TRUE(astStart);
  ASSERT_EQ(astStart->statList()->stat().size(), 3);
  {
    ASSERT_TRUE(astStart->statList()->stat()[0]);
    ASSERT_TRUE(astStart->statList()->stat()[0]->walk());
    ASSERT_FALSE(astStart->statList()->stat()[0]->storeVar());
    SceneParser::WalkFrontContext *walk;
    ASSERT_TRUE(walk = dynamic_cast<SceneParser::WalkFrontContext*>(astStart->statList()->stat()[0]->walk()));
    ASSERT_TRUE(walk->expr());
    SceneParser::VarExprContext *varExpr;
    ASSERT_TRUE(varExpr = dynamic_cast<SceneParser::VarExprContext*>(walk->expr()));
    ASSERT_TRUE(varExpr->var());
    ASSERT_STREQ(varExpr->var()->getText().c_str(), "_step");
  }{
    ASSERT_TRUE(astStart->statList()->stat()[1]->storeVar());
    ASSERT_FALSE(astStart->statList()->stat()[1]->walk());
    ASSERT_TRUE(astStart->statList()->stat()[1]->storeVar()->expr());
    ASSERT_TRUE(astStart->statList()->stat()[1]->storeVar()->var());
    ASSERT_STREQ(astStart->statList()->stat()[1]->storeVar()->var()->getText().c_str(), "_step");
  }{
    ASSERT_TRUE(astStart->statList()->stat()[2]->addVar());
    ASSERT_FALSE(astStart->statList()->stat()[2]->walk());
    {
      ASSERT_TRUE(astStart->statList()->stat()[2]->addVar()->expr());
      SceneParser::VarExprContext *varExpr;
      ASSERT_TRUE(varExpr = dynamic_cast<SceneParser::VarExprContext*>(astStart->statList()->stat()[2]->addVar()->expr()));
      ASSERT_TRUE(varExpr->var());
      ASSERT_STREQ(varExpr->var()->getText().c_str(), "_step");
    }
    ASSERT_TRUE(astStart->statList()->stat()[2]->addVar()->var());
    ASSERT_STREQ(astStart->statList()->stat()[2]->addVar()->var()->getText().c_str(), "_wal5");
  }

  VarVisitor vis;
  auto ret = vis.getVariableContext(astStart);
  ASSERT_EQ(ret.size(), 2);
  auto zwi = ret.extract("_step");
  ASSERT_TRUE(zwi);
  ASSERT_STREQ(zwi.key().c_str(), "_step");

  zwi = ret.extract("_wal5");
  ASSERT_TRUE(zwi);
  ASSERT_STREQ(zwi.key().c_str(), "_wal5");
}
