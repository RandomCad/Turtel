#include <iostream>
#include <regex>

#include "TestCodeGenerator.Helper.h"
#include "../CodeGenerator.h"
#include "UnitTest.h"
#include <string>

bool TestCodeGeneratorProgramBase(TestError *&ret){
  std::stringstream stream;
  CodeGenerator gen(stream);
  stream.str("");
  gen.ProgrammBase();
  std::string testRet;
  testRet = stream.str();
  std::cout << testRet << std::endl;
  
  REGEX_ASSERT(testRet,"(?:.|\\n)*(?:#include\\s+<\\w*\\.h>(?:.|\\n)*){2,}(?:.|\\n)*int main\\(int argc, const char \\*argv\\[\\]\\)\\s*\\{(?:.|\\n)*", ret, 0)
  return false;
}
bool TestCodeGeneratorCTor(TestError *& ret){
  std::stringstream stream;
  CodeGenerator gen(stream);
  std::string testRet;
  testRet = stream.str();
  std::cout << testRet << std::endl;
  
  REGEX_ASSERT(testRet,"(?:.|\\n)*(?:#include\\s+<\\w*\\.h>(?:.|\\n)*){2,}(?:.|\\n)*int main\\(int argc, const char \\*argv\\[\\]\\)\\s*\\{(?:.|\\n)*", ret, 0)
  return false;
}
bool TestCodeGeneratorEndMain(TestError *&){
  return false;
}
bool TestCodeGeneratorDTor(TestError *&){
  return false;
}
