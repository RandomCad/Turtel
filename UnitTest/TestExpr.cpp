#include "../libs/SceneLexer.h"
#include "../libs/SceneParser.h"
#include "MyVisitor.h"
#include "UnitTest.h"
#include "antlr4-runtime.h"
#include "TestExpr.h"
#include <any>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace antlr4;

bool TestExprExec(TestError *&ret){
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
  for (size_t i = 0; i < TestAmount; i++) { //Test Number
    std::cout << "test " << testNumber++ << std::endl;// continue;
    //Setup test data
    double a = rand() / (double)rand();
      
    stream.clear();
    stream << std::to_string(std::abs(a)) << std::endl;
      
    //run test
    ANTLRInputStream input(stream);
    SceneLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SceneParser parser(&tokens);
    MyVisitor vis;
      
    auto test = parser.expr();
    
    //check data
    std::any testVar = test->accept(&vis);
    INT_ASSERT(testVar.type(), typeid(std::string), ret, testNumber);
    std::cout << std::any_cast<std::string>(testVar) << std::endl << std::abs(a) << std::endl;
    STRING_ASSERT(std::any_cast<std::string>(testVar), std::to_string(std::abs(a)), ret, testNumber)
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
    double a = rand() / (double)rand();
      
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
