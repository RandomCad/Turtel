#include <ANTLRInputStream.h>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "TestCodeGenerator.Helper.h"
#include "../src/CodeGenerator.h"
#include "../src/LLVMInterface.h"
#include "../libs/SceneParser.h"
#include "../libs/SceneLexer.h"
#include "UnitTest.h"

using namespace antlr4;

bool TestBasicEmptyMain(TestError *&);
bool TestBasicWalk(TestError *&);

bool TestCodeGenerator(std::stack<TestError *>& col){
  bool ret = false;

  TestError *in;
  
  if (TestBasicEmptyMain(in)){
    col.push(in);
    ret = true;
  }

  if(TestBasicWalk(in)){
    col.push(in);
    ret = true;
  }
    
  return ret;
}

bool TestBasicEmptyMain(TestError *& in ){
  const char * testFile = TEST_OUTPUT_DIR "/EmptyMainTest.out";
  LLVMInterface interface(testFile);
  std::stringstream stream;
  stream 
    << "begin\n"
    << "end\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.file();
  CodeGenerator test(interface.llvmFile, astStart);
  test.GenerateCode();

  interface.CallLLVM();
  
  TRUE_ASSERT(std::filesystem::exists(testFile), in, TestErrorSeveraty::ERROR);
  
  return false;
}

bool TestBasicWalk(TestError *& in ){
  const char *testFile = TEST_OUTPUT_DIR "/BasicWalkTest.out";
  LLVMInterface interface(testFile);
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  walk 5\n"
    << "end\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.file();
  CodeGenerator test(interface.llvmFile, astStart);
  test.GenerateCode();

  interface.CallLLVM();
  TRUE_ASSERT(std::filesystem::exists(testFile), in, TestErrorSeveraty::ERROR);

  return false;
}

bool TestCodeGeneratorEmpty(TestError *&col){
  const char *testFile = TEST_OUTPUT_DIR "/EmptyCodeGenerator.out";
  LLVMInterface interface(testFile);
  CodeGenerator test(interface.llvmFile);
  try {
    test.GenerateCode();
  } catch (std::invalid_argument) {
    return false;   
  }

  col = declareError("NoNullPointer", "A astMain was generated without any ast given to the CodeGenerator Ctor", TestErrorSeveraty::ERROR);
  return true;
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
  //CodeGenerator gen(stream, new SceneParser::FileContext());
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
