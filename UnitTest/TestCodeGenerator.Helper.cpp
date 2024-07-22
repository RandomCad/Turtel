#include <filesystem>
#include <iostream>
#include <string>
#include <regex>

#include "TestCodeGenerator.Helper.h"
#include "../CodeGenerator.h"
#include "LLVMInterface.h"
#include "UnitTest.h"

bool TestCodeGeneratorEmpty(TestError *&col){
  LLVMInterface interface("test.out");
  CodeGenerator test(interface.llvmFile);
  test.GenerateCode();

  interface.CallLLVM();
  if ( !std::filesystem::exists("test.out")){
    col = new TestError(
          std::string(__func__), "The output file doesn't exist. Some thing in the compilation went wrong.", 1, 0);
    return true;
  }

  std::cout << "The correct working of the programm most be tested by hand." << std::endl;
  
  return false;
  
}

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
bool TestCodeGeneratorEndMain(TestError *& ret){
  std::stringstream stream;
  CodeGenerator gen(stream);
  gen.EndeMain();
  std::string testRet;
  testRet = stream.str();
  std::cout << testRet << std::endl;
  
  REGEX_ASSERT(testRet,"(.|\\n)*int main\\(int argc, const char \\*argv\\[\\]\\)\\{(.|\\n)*\\/\\/(\\w|\\s)+\\n\\}(.|\\n)*", ret, 0)
  return false;

  return false;
}
bool TestCodeGeneratorDTor(TestError *&){
  return false;
}
