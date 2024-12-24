#include "src/TopLevelVisitor.h"
#include "gtest/gtest.h"
#include <ANTLRInputStream.h>
#include <any>
#include <gtest/gtest.h>

#include "../libs/SceneParser.h"
#include "../libs/SceneLexer.h"
#include "src/VariableHeandler.h"

using  namespace antlr4;

TEST(TopLevelVisitor, AcceptNumContext){
  std::stringstream stream;
  stream 
    << "5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.number();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);
  ASSERT_FALSE(ret.has_value());
}

TEST(TopLevelVisitor, WalkVisit){
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
  callRet = astStart->accept(&toTest);
    
  ASSERT_FALSE(callRet.has_value());

  ret.flush();

  std::cerr << ret.str() << std::endl;
  ASSERT_EQ(
      ret.str(), 
      "  SDL_RenderDrawLine(__env_rnd, __env_posX, __env_posY, __env_posX + 5 * cos(__env_rot), __env_posY + 5 * sin(__env_rot));\n" 
      );
  
}
