#include "../libs/SceneLexer.h"
#include "../libs/SceneParser.h"
#include "src/MathVisitor.h"
#include "src/VariableHeandler.h"
#include "src/TopLevelVisitor.h"
#include <gtest/gtest.h>
#include <cmath>

using namespace antlr4;

TEST(MathVisitor, NumExpr){
  std::stringstream stream;
  stream << "794651" << std::endl << "91231" << std::endl << "6541.89645" << std::endl
    << "786451.168754" << std::endl << ".8765413" << std::endl 
    << ".876541132" << std::endl;

  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  MathVisitor visitor(var);

  auto test = parser.number();
  ASSERT_TRUE(test);
  int testNum = std::any_cast<int64_t>(test->accept(&visitor));
  ASSERT_EQ(testNum, 794651);

  test = parser.number();
  ASSERT_TRUE(test);
  testNum = std::any_cast<int64_t>(test->accept(&visitor));
  ASSERT_EQ(testNum, 91231);

  test = parser.number();
  ASSERT_TRUE(test);
  double testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, 6541.89645);

  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, 786451.168754);
  
  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, .8765413);

  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, .876541132);
}
#if FALSE
bool TestWalkParsing(TestError *&ret){
  std::stringstream stream;
  size_t testNumber = 0;

  //add data
  stream << "walk 5" << std::endl;
  stream << "walk -5walk 20 + 5" << std::endl;
  
  //Test preperation
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  //test
  auto test = parser.walk();
  NOT_NULL_ASSERT(test, ret, testNumber);
  NOT_NULL_ASSERT(test->expr(), ret, testNumber);

  test = parser.walk();
  NOT_NULL_ASSERT(test, ret, ++testNumber);
  NOT_NULL_ASSERT(test->expr(), ret, testNumber);
  NOT_NULL_ASSERT(dynamic_cast<SceneParser::NegateContext*>(test->expr()), ret, testNumber)

  test = parser.walk();
  NOT_NULL_ASSERT(test, ret, ++testNumber);
  NOT_NULL_ASSERT(test->expr(), ret, testNumber);
  NOT_NULL_ASSERT(dynamic_cast<SceneParser::AddContext*>(test->expr()), ret, testNumber)

  return false;
}

bool TestVariableParsing(TestError *&ret){
  const size_t TestAmount = 1000;
  std::stringstream stream;
  std::string cor[TestAmount];

  for (size_t i = 0; i < TestAmount; i++) {
    cor[i] = RandomString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", 1);
    cor[i] += RandomString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@", rand() % (TestAmount)); 

    stream << cor[i] << std::endl;
  }
  {  
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  for (size_t i = 0; i < TestAmount; i++) {
    std::cout << "test" << i << std::endl;
    auto test = parser.var();
    
    NOT_NULL_ASSERT(test, ret, i);
    SceneParser::VariableContext *testVar = dynamic_cast<SceneParser::VariableContext*>(test); 
    std::cout << testVar->ID()->getSymbol()->getText() << std::endl << cor[i] << std::endl;
    NOT_NULL_ASSERT(testVar, ret, i)
    NULL_ASSERT(testVar->exception, ret, i);
    NOT_NULL_ASSERT(testVar->ID(), ret, i);
    NOT_NULL_ASSERT(testVar->ID()->getSymbol(), ret, i)
    STRING_ASSERT(testVar->ID()->getSymbol()->getText(), cor[i], ret, i)
  }
  }

  stream.clear();

  //explisitly test the case of only @
  cor[0] = "@";
  stream << cor[0] << std::endl;

  for (size_t i = 1; i < TestAmount; i++) {
    cor[i] = "@";
    cor[i] += RandomString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@", rand() % (TestAmount)); 

    stream << cor[i] << std::endl;
  }

  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  for (size_t i = 0; i < TestAmount; i++) {
    std::cout << "test" << i << std::endl;
    auto test = parser.var();
    
    NOT_NULL_ASSERT(test, ret, i);
    SceneParser::GlobalVariableContext *testVar = dynamic_cast<SceneParser::GlobalVariableContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, i)
    NULL_ASSERT(testVar->exception, ret, i);
    NOT_NULL_ASSERT(testVar->IncID(), ret, i);
    NOT_NULL_ASSERT(testVar->IncID()->getSymbol(), ret, i)
    STRING_ASSERT(testVar->IncID()->getSymbol()->getText(), cor[i], ret, i)
  }


  
  return false;
}

#endif

TEST(MathVisitor, ConstFoldingNaN) {
  std::stringstream stream;
  stream << "0.0 / 0.0";
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto exprCtx = parser.expr();
  ASSERT_TRUE(exprCtx != nullptr);
  
  VariableHeandler var;
  std::stringstream dummyOutput;
  TopLevelVisitor visitor(dummyOutput, var);
  
  std::string resultStr = visitor.UnwrapExpre(exprCtx);
  
  //convertion for case-insensitive checking:
  std::transform(resultStr.begin(), resultStr.end(), resultStr.begin(), 
                 [](unsigned char c){ return std::toupper(c); });
  
  std::cerr << "Unwrapped expression (uppercase): " << resultStr << std::endl;
  
  ASSERT_TRUE(resultStr.find("NAN") != std::string::npos)
      << "Expected 'NAN' in the unwrapped expression but got: " << resultStr;
}

