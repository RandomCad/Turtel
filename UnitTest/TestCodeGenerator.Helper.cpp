#include <ANTLRInputStream.h>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>

#include "TestCodeGenerator.Helper.h"
#include "../CodeGenerator.h"
#include "LLVMInterface.h"
#include "SceneParser.h"
#include "SceneLexer.h"
#include "UnitTest.h"

using namespace antlr4;

bool TestBasicEmptyMain(TestError *);
bool TestBasicWalk(TestError *);

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

bool TestBasicEmptyMain(TestError * in ){
  LLVMInterface interface("TEST_OUTPUT_DIR" "EmptyMainTest.out");
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
  if ( !std::filesystem::exists("TEST_OUTPUT_DIR" "EmptyMainTest.out")){
    in = declareError("FileDosNotExist", "The output file doesn't exist. Some thing in the compilation went wrong.", TestErrorSeveraty::ERROR);
    return true;
  }
  return false;
}

bool TestBasicWalk(TestError * in ){
  LLVMInterface interface("TEST_OUTPUT_DIR" "BasicWalkTest.out");
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
  if ( !std::filesystem::exists("TEST_OUTPUT_DIR" "BasicWalkTest.out")){
    in = declareError("FileDosnotExist", "The output file doesn't exist", TestErrorSeveraty::ERROR); 
    return true;
  }
  return false;
}

bool TestCodeGeneratorEmpty(TestError *&col){
  LLVMInterface interface("test.out");
  CodeGenerator test(interface.llvmFile);
  test.GenerateCode();

  interface.CallLLVM();
  if ( !std::filesystem::exists("test.out")){
    col = declareError("FileDosnotExist", "The output file doesn't exist", TestErrorSeveraty::ERROR); 
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
