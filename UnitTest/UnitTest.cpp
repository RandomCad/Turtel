#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <filesystem>
#include <stack>



#include "../src/CodeGenerator.h"

#include "TestVariable.h"
#include "UnitTest.h"
#include "TestExpr.h"
#include "TestCodeGenerator.Helper.h"
#include "TestLLVMInterface.h"
#include "TestMathVisitor.h"


int main(int argc, const char *argv[]){
  srand(0);
  std::filesystem::create_directory(TEST_OUTPUT_DIR);

  std::stack<TestError *> colector;
  TestError * next;
  if(TestNumberParsing(next)) colector.push(next);
  if(TestNumberExec(next)) colector.push(next);
  if(TestVariableParsing(next)) colector.push(next);
  if(TestExprParsing(next)) colector.push(next);
  if(TestExprExec(next)) colector.push(next);
  if(TestWalkParsing(next)) colector.push(next);
  if(TestCodeGeneratorProgramBase(next)) colector.push(next);
  if(TestCodeGeneratorEndMain(next)) colector.push(next);
  if(TestCodeGeneratorCTor(next)) colector.push(next);
  if(TestCodeGeneratorDTor(next)) colector.push(next);
  TestVariable(colector);
  TestLLVMInterface(colector);
  if(TestCodeGeneratorEmpty(next)) colector.push(next);
  TestCodeGenerator(colector);

  int maxErr = 0;
  if (colector.empty()) return 0;
  std::cout << colector.size();
  do {
    TestError *i = colector.top();
    maxErr = (maxErr > i->severity) ? maxErr : i->severity;
    std::cout << *i << std::endl;
  }while (colector.empty());

  return (maxErr) ? 1: 0;

}

TestError::TestError(const char *name,const char *errorName, const char * desc, TestErrorSeveraty sev, int line):
    testName(name), errName(errorName), 
    errDescription(desc), severity(sev), lineNum(line) {
}

std::ostream &operator<<(std::ostream &a, TestError &b){
  a << "The test " << b.testName << " created a " << b.severity << "\n"
    << "The name is " << b.errName << ". With the text:\n"
    << b.errDescription << "\n"
    << "The " << b.severity << " was created on line " << b.lineNum << std::endl;
  return a;
}

std::string RandomString(const char val[], const size_t len){
  std::string ret;
  //ret.reserve(len);
  for (int i = 0; i < len; ++i) {
    ret += val[rand() % strlen(val)];
  }
  return ret;
}
