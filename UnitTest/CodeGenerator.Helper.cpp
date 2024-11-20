#include <ANTLRInputStream.h>
#include <any>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <sstream>

#include "../src/CodeGenerator.h"
#include "../src/LLVMInterface.h"
#include "../libs/SceneParser.h"
#include "../libs/SceneLexer.h"
#include "gtest/gtest.h"

using namespace antlr4;

TEST(Code_Generator_Test, TestEmptyMainVisit){
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
  EXPECT_TRUE(astStart);
  EXPECT_TRUE(astStart->main());
  EXPECT_EQ(astStart->calcdef().size(), 0);
  EXPECT_EQ(astStart->pathdef().size(), 0);

  std::stringstream testOut;
  CodeGenerator test(testOut, astStart);
  astStart->accept(&test);
  ASSERT_NE(testOut.str(), "");
  ASSERT_EQ(testOut.str(), std::string("void TurtelMain(SDL_Renderer * __rnd_rnd){\n  double __env_posX;\n  double __env_posY;\n}\n\n"));


}

TEST(Code_Generator_Test, BasicEmptyMain){
  const char * testFile = "EmptyMainTest.out";
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
  EXPECT_TRUE(astStart);
  EXPECT_TRUE(astStart->main());
  EXPECT_EQ(astStart->calcdef().size(), 0);
  EXPECT_EQ(astStart->pathdef().size(), 0);

  
  CodeGenerator test(interface.llvmFile, astStart);
  test.GenerateCode();

  interface.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));
  
}
/*

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
}//*/
