#include "TestMathVisitor.h"

#include "../libs/SceneLexer.h"
#include "../libs/SceneParser.h"
#include "tree/TerminalNode.h"
#include "../src/CodeGenerator.h"

using namespace antlr4;

bool TestNumberExec(TestError*&ret){
  size_t testNumber = 0;
  std::stringstream stream;
  stream << "794651" << std::endl << "91231" << std::endl << "6541.89645" << std::endl
    << "786451.168754" << std::endl << ".8765413" << std::endl 
    << ".876541132" << std::endl;

  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  CodeGenerator visitor;

  auto test = parser.number();
  int testNum = std::any_cast<int64_t>(test->accept(&visitor));
  INT_ASSERT(testNum, 794651, ret, testNumber++)

  test = parser.number();
  testNum = std::any_cast<int64_t>(test->accept(&visitor));
  INT_ASSERT(testNum, 91231, ret, testNumber++)

  test = parser.number();
  NOT_NULL_ASSERT(dynamic_cast<SceneParser::FloatContext*>(test), ret, testNumber)
  double testDouble = std::any_cast<double>(test->accept(&visitor));
  INT_ASSERT(testDouble, 6541.89645, ret, testNumber++)

  test = parser.number();
  testDouble = std::any_cast<double>(test->accept(&visitor));
  INT_ASSERT(testDouble, 786451.168754, ret, testNumber);
  
  test = parser.number();
  testDouble = std::any_cast<double>(test->accept(&visitor));
  INT_ASSERT(testDouble, .8765413, ret, testNumber)

  test = parser.number();
  testDouble = std::any_cast<double>(test->accept(&visitor));
  INT_ASSERT(testDouble, .876541132, ret, testNumber)
  
  return false;
}

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

bool TestNumberParsing(TestError *&ret){
  std::stringstream stream;
  stream << "794651" << std::endl << "91231" << std::endl << "6541.89645" << std::endl
    << "786451.168754" << std::endl << ".8765413" << std::endl 
    << ".876541132" << std::endl;
  
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  auto test = parser.number();
  NOT_NULL_ASSERT(test, ret, 1);
  auto testVar = dynamic_cast<SceneParser::IntContext*>(test);
  NOT_NULL_ASSERT(testVar, ret, 1);
  NOT_NULL_ASSERT(testVar->Num()->getSymbol(),ret, 1)
  Token *testToken = testVar->Num()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("794651"), ret, 1)

  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 1);
  testVar = dynamic_cast<SceneParser::IntContext*>(test);
  NOT_NULL_ASSERT(testVar, ret, 1);
  NOT_NULL_ASSERT(testVar->Num()->getSymbol(),ret, 1)
  testToken = testVar->Num()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("91231"), ret, 1)
  
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 1);
  auto testFloat = dynamic_cast<SceneParser::FloatContext*>(test);
  NOT_NULL_ASSERT(testFloat, ret, 1);
  NOT_NULL_ASSERT(testFloat->Float()->getSymbol(),ret, 1)
  testToken = testFloat->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("6541.89645"), ret, 1)
 
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 1);
  testFloat = dynamic_cast<SceneParser::FloatContext*>(test);
  NOT_NULL_ASSERT(testFloat, ret, 1);
  NOT_NULL_ASSERT(testFloat->Float()->getSymbol(),ret, 1)
  testToken = testFloat->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("786451.168754"), ret, 1)
   
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 1);
  testFloat = dynamic_cast<SceneParser::FloatContext*>(test);
  NOT_NULL_ASSERT(testFloat, ret, 1);
  NOT_NULL_ASSERT(testFloat->Float()->getSymbol(),ret, 1)
  testToken = testFloat->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string(".8765413"), ret, 1)
    
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 1);
  testFloat = dynamic_cast<SceneParser::FloatContext*>(test);
  NOT_NULL_ASSERT(testFloat, ret, 1);
  NOT_NULL_ASSERT(testFloat->Float()->getSymbol(),ret, 1)
  testToken = testFloat->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string(".876541132"), ret, 1)
  
  return false;
} 


