#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <forward_list>
#include <tree/ParseTreeType.h>
#include <tree/TerminalNode.h>

#include "antlr4-runtime.h"
#include "libs/SceneLexer.h"
#include "libs/SceneParser.h"

using namespace antlr4;

#define NOT_NULL_ASSERT(ptr, ret, num) if( ptr == nullptr){ \
                              ret = new TestError(std::string(__func__), "not nullptr assert fail.", 1, num);  \
                              return true; \
                              } 

#define NULL_ASSERT(ptr, ret, num) if( ptr != nullptr){ \
                              ret = new TestError(std::string(__func__), "nullptr assert fail.", 1, num);  \
                              return true; \
                              } 

#define STRING_ASSERT(str1, str2, ret, num) \
  if(str1.compare(str2) != 0){ \
  ret = new TestError(std::string(__func__), "String assert fail.", 1, num); return true; }
#define INT_ASSERT(int1, int2, ret, num) \
  if(int1 != int2){\
    ret = new TestError(std::string(__func__), "Int equal assert fail.", 1, num); return true; }


struct TestError {
  std::string testName;
  std::string errName;
  int sev;
  int num;
  TestError(std::string &name, std::string &err, int severity): testName(name), errName(err), sev(severity) {}
  TestError(std::string &&name, std::string &&err, int severity, int Num = -1): testName(name),
    errName(err), sev(severity), num(Num) {}
};

std::string RandomString(const char val[], const size_t len);
bool TestNumberParsing(TestError*&);
bool TestVariableParsing(TestError *&ret);
bool TestExprParsing(TestError *&ret);
bool TestWalkParsing(TestError *&ret);

int main(int argc, const char *argv[]){
  srand(0);
  std::forward_list<TestError*> colector;

  TestError *next = nullptr;
  if(argc <=2){
    std::cerr << "No argument was given. Test wasn't correctly set up";
    return 1;
  }
  
  std::cout << "running test number: " << argv[2] << std::endl;
  
  switch (atoi(argv[2])) {
    case 0:
      if(TestNumberParsing(next)) colector.push_front(next);
      break;

    case 1:
      if(TestVariableParsing(next)) colector.push_front(next);
      break;
    
    case 2:
      if( TestExprParsing(next)) colector.push_front(next);
      break;
    case 3:
      if( TestWalkParsing(next)) colector.push_front(next);
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:

    default:
      std::cerr << "Tryed to run non existing test. Test wasn't correctly set up";
      return -1;
  }

  int maxErr = 0;

  for(TestError *var : colector) {
    maxErr = (maxErr > var->sev) ? maxErr : var->sev;
    std::cout << "Error in function" << var->testName << std::endl << var->errName << std::endl
              << "Error in the " << var->num << " test." << std::endl;
  }

  return maxErr;

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

bool TestExprParsing(TestError *&ret){
  const size_t TestAmount = 1000;
  std::stringstream stream;
  size_t testNumber = 0;
  
  { //test Exponent
    //creat test TestData
    int cor[TestAmount][2];
    for (int i = 0; i < TestAmount; ++i) {
      cor[i][0] = rand();
      cor[i][1] = rand();
      stream << cor[i][0] << '^' << cor[i][1] << std::endl;
    }
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);    
  
    for (size_t i = 0; i < TestAmount; i++) {
      std::cout << "test " << testNumber++ << std::endl;
      auto test = parser.expr();
    
      NOT_NULL_ASSERT(test, ret, testNumber);
      auto *testVar = dynamic_cast<SceneParser::ExpContext*>(test); 
      NOT_NULL_ASSERT(testVar, ret, testNumber)
      NULL_ASSERT(testVar->exception, ret, testNumber);
      auto testVec = testVar->children;
      INT_ASSERT(testVec.size(), 3, ret, testNumber);
      NOT_NULL_ASSERT(testVec[0], ret, testNumber);
      NOT_NULL_ASSERT(testVec[1], ret, testNumber);
      NOT_NULL_ASSERT(testVec[2], ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[0]), ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1]), ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[2]), ret, testNumber);
      STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1])->getSymbol()->getText(), "^", ret, testNumber);
    }
  }
  { //test Multiplikation
    //creat test TestData
    int cor[TestAmount][2];
    for (int i = 0; i < TestAmount; ++i) {
      cor[i][0] = rand();
      cor[i][1] = rand();
      stream << cor[i][0] << '*' << cor[i][1] << std::endl;
    }
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);    
  
    for (size_t i = 0; i < TestAmount; i++) {
      std::cout << "test " << testNumber++ << std::endl;
      auto test = parser.expr();
    
      NOT_NULL_ASSERT(test, ret, testNumber);
      auto *testVar = dynamic_cast<SceneParser::MultContext*>(test); 
      NOT_NULL_ASSERT(testVar, ret, testNumber)
      NULL_ASSERT(testVar->exception, ret, testNumber);
      auto testVec = testVar->children;
      INT_ASSERT(testVec.size(), 3, ret, testNumber);
      NOT_NULL_ASSERT(testVec[0], ret, testNumber);
      NOT_NULL_ASSERT(testVec[1], ret, testNumber);
      NOT_NULL_ASSERT(testVec[2], ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[0]), ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1]), ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[2]), ret, testNumber);
      STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1])->getSymbol()->getText(), "*", ret, testNumber);
    }
  }
  { //test Difide
    //creat test TestData
    int cor[TestAmount][2];
    for (int i = 0; i < TestAmount; ++i) {
      cor[i][0] = rand();
      cor[i][1] = rand();
      stream << cor[i][0] << '/' << cor[i][1] << std::endl;
    }
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);    
  
    for (size_t i = 0; i < TestAmount; i++) {
      std::cout << "test " << testNumber++ << std::endl;
      auto test = parser.expr();
    
      NOT_NULL_ASSERT(test, ret, testNumber);
      auto *testVar = dynamic_cast<SceneParser::DifeContext*>(test); 
      NOT_NULL_ASSERT(testVar, ret, testNumber)
      NULL_ASSERT(testVar->exception, ret, testNumber);
      auto testVec = testVar->children;
      INT_ASSERT(testVec.size(), 3, ret, testNumber);
      NOT_NULL_ASSERT(testVec[0], ret, testNumber);
      NOT_NULL_ASSERT(testVec[1], ret, testNumber);
      NOT_NULL_ASSERT(testVec[2], ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[0]), ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1]), ret, testNumber);
      NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[2]), ret, testNumber);
      STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1])->getSymbol()->getText(), "/", ret, testNumber);
    }       
  }
  for (size_t i = 0; i < TestAmount; i++) { //Test Add
    //Setup test data
    int a = rand();
    int b = rand();
      
    stream.clear();
    stream << a << '+' << b << std::endl;
      
    //run test
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);
      
    std::cout << "test " << testNumber++ << std::endl;
    auto test = parser.expr();
    
    //check data
    NOT_NULL_ASSERT(test, ret, testNumber);
    auto *testVar = dynamic_cast<SceneParser::AddContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, testNumber)
    NULL_ASSERT(testVar->exception, ret, testNumber);
    auto testVec = testVar->children;
    INT_ASSERT(testVec.size(), 3, ret, testNumber);
    NOT_NULL_ASSERT(testVec[0], ret, testNumber);
    NOT_NULL_ASSERT(testVec[1], ret, testNumber);
    NOT_NULL_ASSERT(testVec[2], ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(testVec[0]), ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1]), ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(testVec[2]), ret, testNumber);
    STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1])->getSymbol()->getText(), "+", ret, testNumber);
  }
  for (size_t i = 0; i < TestAmount; i++) { //Test Add
    std::cout << "test " << testNumber++ << std::endl;// continue;
    //Setup test data
    int a = rand();
    int b = rand();
      
    stream.clear();
    std::string cor;
    cor += std::to_string(a);
    cor += '-';
    cor += std::to_string(b);
    stream << cor << std::endl;
      
    //run test
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);
      
    auto test = parser.expr();
    
    //check data
    NOT_NULL_ASSERT(test, ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::DimContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(test), ret, testNumber);
    auto *testVar = dynamic_cast<SceneParser::DimContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, testNumber)
    NULL_ASSERT(testVar->exception, ret, testNumber);
    auto testVec = testVar->children;
    INT_ASSERT(testVec.size(), 3, ret, testNumber);
    NOT_NULL_ASSERT(testVec[0], ret, testNumber);
    NOT_NULL_ASSERT(testVec[1], ret, testNumber);
    NOT_NULL_ASSERT(testVec[2], ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(testVec[0]), ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1]), ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(testVec[2]), ret, testNumber);
    STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[1])->getSymbol()->getText(), "-", ret, testNumber);
  }
  for (size_t i = 0; i < TestAmount; i++) { //Test Add
    std::cout << "test " << testNumber++ << std::endl;// continue;
    //Setup test data
    int a = rand();
      
    stream.clear();
    stream << '|' << a << '|' << std::endl;
      
    //run test
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);
      
    auto test = parser.expr();
    
    //check data
    NOT_NULL_ASSERT(test, ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::DimContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(test), ret, testNumber);
    auto *testVar = dynamic_cast<SceneParser::ABSContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, testNumber)
    NULL_ASSERT(testVar->exception, ret, testNumber);
    auto testVec = testVar->children;
    INT_ASSERT(testVec.size(), 3, ret, testNumber);
    NOT_NULL_ASSERT(testVec[0], ret, testNumber);
    NOT_NULL_ASSERT(testVec[1], ret, testNumber);
    NOT_NULL_ASSERT(testVec[2], ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[0]), ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(testVec[1]), ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[2]), ret, testNumber);
    STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[0])->getSymbol()->getText(), "|", ret, testNumber);
    STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[2])->getSymbol()->getText(), "|", ret, testNumber);
  }
  for (size_t i = 0; i < TestAmount; i++) { //Test Add
    std::cout << "test " << testNumber++ << std::endl;// continue;
    //Setup test data
    int a = rand();
      
    stream.clear();
    stream << '-' << std::abs(a) << std::endl;
      
    //run test
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);
      
    auto test = parser.expr();
    
    //check data
    NOT_NULL_ASSERT(test, ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::DimContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumExprContext*>(test), ret, testNumber);
    auto *testVar = dynamic_cast<SceneParser::NegateContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, testNumber)
    NULL_ASSERT(testVar->exception, ret, testNumber);
    NOT_NULL_ASSERT(testVar->number(), ret, testNumber)
    auto testVec = testVar->children;
    INT_ASSERT(testVec.size(), 2, ret, testNumber);
    NOT_NULL_ASSERT(testVec[0], ret, testNumber);
    NOT_NULL_ASSERT(testVec[1], ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[0]), ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[1]), ret, testNumber);
    STRING_ASSERT(dynamic_cast<antlr4::tree::TerminalNode*>(testVec[0])->getSymbol()->getText(), "-", ret, testNumber);
  }
  for (size_t i = 0; i < TestAmount; i++) { //Test Add
    std::cout << "test " << testNumber++ << std::endl;// continue;
    //Setup test data
    int a = rand();
      
    stream.clear();
    stream << std::abs(a) << std::endl;
      
    //run test
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);
      
    auto test = parser.expr();
    
    //check data
    NOT_NULL_ASSERT(test, ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::DimContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(test), ret, testNumber);
    auto *testVar = dynamic_cast<SceneParser::NumExprContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, testNumber);
    NULL_ASSERT(testVar->exception, ret, testNumber);
    NOT_NULL_ASSERT(testVar->number(), ret, testNumber)
    auto testVec = testVar->children;
    INT_ASSERT(testVec.size(), 1, ret, testNumber);
    NOT_NULL_ASSERT(testVec[0], ret, testNumber);
    NOT_NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(testVec[0]), ret, testNumber);
  }
  for (size_t i = 0; i < TestAmount; i++) { //Test Add
    std::cout << "test " << testNumber++ << std::endl;// continue;
    //Setup test data
    int a = rand();
      
    stream.clear();
    stream << RandomString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", TestAmount) << std::endl;
      
    //run test
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);
      
    auto test = parser.expr();
    
    //check data
    NOT_NULL_ASSERT(test, ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::DimContext*>(test), ret, testNumber);
    NULL_ASSERT(dynamic_cast<SceneParser::NumberContext*>(test), ret, testNumber);
    auto *testVar = dynamic_cast<SceneParser::VarExprContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, testNumber);
    NULL_ASSERT(testVar->exception, ret, testNumber);
    NOT_NULL_ASSERT(testVar->var(), ret, testNumber);
    auto testVec = testVar->children;
    INT_ASSERT(testVec.size(), 1, ret, testNumber);
    NOT_NULL_ASSERT(testVec[0], ret, testNumber);
  }
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
  std::ifstream stream;
  stream.open("../UnitTest/TestData/Numbers.test");
  
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  auto test = parser.number();
  NOT_NULL_ASSERT(test, ret, 1);
  NOT_NULL_ASSERT(test->Num(), ret, 1);
  NULL_ASSERT(test->Float(), ret, 1)
  NOT_NULL_ASSERT(test->Num()->getSymbol(),ret, 1)
  Token *testToken = test->Num()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("794651"), ret, 1)

  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 2);
  NOT_NULL_ASSERT(test->Num(), ret, 2);
  NULL_ASSERT(test->Float(), ret, 2)
  NOT_NULL_ASSERT(test->Num()->getSymbol(),ret, 2)
  testToken = test->Num()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("91231"), ret, 2)
 
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 3);
  NULL_ASSERT(test->Num(), ret, 3);
  NOT_NULL_ASSERT(test->Float(), ret, 3)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret, 3)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("6541.89645"), ret, 3) 
 
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 4);
  NULL_ASSERT(test->Num(), ret, 4);
  NOT_NULL_ASSERT(test->Float(), ret, 4)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret, 4)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("786451.168754"), ret, 4) 
   
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 5);
  NULL_ASSERT(test->Num(), ret, 5);
  NOT_NULL_ASSERT(test->Float(), ret, 5)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret, 5)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string(".8765413"), ret, 5) 
  
  test = parser.number();
  NOT_NULL_ASSERT(test, ret, 6);
  NULL_ASSERT(test->Num(), ret, 6);
  NOT_NULL_ASSERT(test->Float(), ret, 6)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret, 6)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string(".876541132"), ret, 6) 
  
  return false;
} 

std::string RandomString(const char val[], const size_t len){
  std::string ret;
  //ret.reserve(len);
  for (int i = 0; i < len; ++i) {
    ret += val[rand() % strlen(val)];
  }
  return ret;
}
