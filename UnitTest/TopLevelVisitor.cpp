#include "src/TopLevelVisitor.h"
#include <ANTLRInputStream.h>
#include <any>
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>

#include "../libs/SceneParser.h"
#include "../libs/SceneLexer.h"
#include "src/VariableHeandler.h"

using  namespace antlr4;

TEST(Code_Generator, WalkVisit){
  std::stringstream stream;
  stream 
    << "walk 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.walk();
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(astStart->expr());
  ASSERT_EQ(typeid(astStart->expr()), typeid(SceneParser::ExprContext*));
  auto numExpr = dynamic_cast<SceneParser::NumExprContext*>( astStart->expr());
  ASSERT_TRUE(numExpr);
  ASSERT_TRUE(numExpr->number());
  ASSERT_EQ(numExpr->number()->getText(), "5");

  std::stringstream ret;
  VariableHeandler var;
  TopLevelVisitor toTest(ret, var);
  std::any callRet;
  ASSERT_THROW(
    callRet = astStart->accept(&toTest),
    const char *
    );
    
  ASSERT_FALSE(callRet.has_value());

  ASSERT_EQ(
      ret.str(), 
      "  SDL_RenderDrawLine(rnd, __env_posX, __env_posY, __env_posX + 5 * cos(__env_rot), __env_posY + 5 * sin(__env_rot)" 
      );
  
}
