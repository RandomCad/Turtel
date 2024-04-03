#include <iostream>
#include <string>
#include <forward_list>

#include "antlr4-runtime.h"
#include "libs/SceneLexer.h"
#include "libs/SceneParser.h"

using namespace antlr4;

struct TestError {
  std::string testName;
  std::string errName;
  int sev;
  TestError(std::string &name, std::string &err, int severity): testName(name), errName(err), sev(severity) {}
  TestError(std::string &&name, std::string &&err, int severity): testName(name), errName(err), sev(severity) {}
};

bool TestNumberParsing(TestError*&);

int main(int argc, const char *argv[]){
  std::forward_list<TestError*> colector;

  TestError *next = nullptr;
  if(argc <=2){
    std::cerr << "No argument was given. Test wasn't correctly set up";
    return 1;
  }
  
  std::cout << argv[1];
  
  switch (atoi(argv[2])) {
    case 0:
      if(TestNumberParsing(next)) colector.push_front(next);
      break;

    default:
      std::cerr << "Tryed to run non existing test. Test wasn't correctly set up";
      return -1;
  }

  int maxErr = 0;

  for(TestError *var : colector) {
    maxErr = (maxErr > var->sev) ? maxErr : var->sev;
    std::cout << "Error in function" << var->testName << std::endl << var->errName << std::endl;
  }

  return maxErr;
}

#define NOT_NULL_ASSERT(ptr, ret) if( ptr == nullptr){ \
                              ret = new TestError(std::string(__func__), "not nullptr assert fail.", 1);  \
                              return true; \
                              } 

#define NULL_ASSERT(ptr, ret) if( ptr != nullptr){ \
                              ret = new TestError(std::string(__func__), "nullptr assert fail.", 1);  \
                              return true; \
                              } 

#define STRING_ASSERT(str1, str2, ret) \
  if(str1.compare(str2) != 0){ \
  ret = new TestError(std::string(__func__), "String assert fail.", 1); return true; }

bool TestNumberParsing(TestError *&ret){
  std::ifstream stream;
  stream.open("../UnitTest/TestData/Numbers.test");
  
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  auto test = parser.number();
  NOT_NULL_ASSERT(test, ret);
  NOT_NULL_ASSERT(test->Num(), ret);
  NULL_ASSERT(test->Float(), ret)
  NOT_NULL_ASSERT(test->Num()->getSymbol(),ret)
  Token *testToken = test->Num()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("794651"), ret)

  test = parser.number();
  NOT_NULL_ASSERT(test, ret);
  NOT_NULL_ASSERT(test->Num(), ret);
  NULL_ASSERT(test->Float(), ret)
  NOT_NULL_ASSERT(test->Num()->getSymbol(),ret)
  testToken = test->Num()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("-91231"), ret)
 
  test = parser.number();
  NOT_NULL_ASSERT(test, ret);
  NULL_ASSERT(test->Num(), ret);
  NOT_NULL_ASSERT(test->Float(), ret)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("6541.89645"), ret) 
 
  test = parser.number();
  NOT_NULL_ASSERT(test, ret);
  NULL_ASSERT(test->Num(), ret);
  NOT_NULL_ASSERT(test->Float(), ret)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("-786451.168754"), ret) 
   
  test = parser.number();
  NOT_NULL_ASSERT(test, ret);
  NULL_ASSERT(test->Num(), ret);
  NOT_NULL_ASSERT(test->Float(), ret)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string("-.8765413"), ret) 
  
  test = parser.number();
  NOT_NULL_ASSERT(test, ret);
  NULL_ASSERT(test->Num(), ret);
  NOT_NULL_ASSERT(test->Float(), ret)
  NOT_NULL_ASSERT(test->Float()->getSymbol(),ret)
  testToken = test->Float()->getSymbol();
  STRING_ASSERT(testToken->getText(),std::string(".876541132"), ret) 
  
  return false;
} 

