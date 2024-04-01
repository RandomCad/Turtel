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

bool TestNumberParsing(TestError*);

int main(void){
  std::forward_list<TestError*> colector;

  TestError *next = nullptr;
  if(TestNumberParsing(next)) colector.push_front(next);


  int maxErr = 0;

  for(auto var : colector) {
    maxErr = (maxErr > var->sev) ? maxErr : var->sev;
    std::cout << "Error in function" << var->testName << std::endl << var->errName << std::endl;
  }

  return maxErr;
}


bool TestNumberParsing(TestError *ret){
  std::ifstream stream;
  stream.open("../UnitTest/TestData/Numbers.test");
  
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  auto test = parser.number();
  if(test == nullptr){
    ret = new TestError(std::string(__func__), "returned a nullptr", 1); 
    return true;
  }

  

  return false;
} 

