#include <ANTLRInputStream.h>
#include <cstdlib>
#include <functional>
#include <gtest/gtest.h>
#include <istream>
#include <iterator>
#include <llvm/Support/Chrono.h>
#include <regex>
#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>

#include "../src/CodeGenerator.h"
#include "../src/LLVMInterface.h"
#include "../libs/SceneParser.h"
#include "../libs/SceneLexer.h"
#include "gtest/gtest.h"
#include "TestSuits.h"

using namespace antlr4;

TEST(CodeGeneratorTestSuite, TestVarCommands){
  const char * testFile = "TestVarCommand.out";
  std::filesystem::remove(testFile);

  LLVMInterface interface(testFile);
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  store 5 in test\n"
    << "  store -3 in ret\n"
    << "  add ret to test\n"  //test = 2
    << "  mul ret by test\n"  //ret = -3 * 2 = -6
    << "  div test by ret\n"  //test = 2 / -6 = -0,5
    << "  store ret in test\n"//test = ret = -6
    << "  mul ret by test\n"  //ret = 36
    << "  finish ret\n"
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

  std::istream &toTest(interface.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  interface.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  //couldn't finde a way to do this direcktly in cpp
  int exitCode = std::system((std::string("./") + testFile /*+ std::string("\nif [ $? -eq 36 ]; then exit 0; else exit 1; fi")*/).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 36);
  //TODO: check the output
}//*/

TEST(CodeGeneratorTestSuite, TestTrivialSave){
  const char * testFile = "TestTrivialSave.out";
  std::filesystem::remove(testFile);

  LLVMInterface interface(testFile);
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  save test\n"
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

  std::istream &toTest(interface.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  interface.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  //TODO: check the output
}

TEST(CodeGeneratorTestSuite, TestEmptyMainVisit){
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
  std::cerr << "setupt finished" << std::endl;
  astStart->accept(&test);
  std::cerr << "running the checks" << std::endl;
  ASSERT_FALSE(testOut.bad());
  ASSERT_NE(testOut.str(), "");

  testOut.seekg(0);
  std::string buf;
  ///check for main definition
  std::getline(testOut, buf);
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "void\\s+TurtelMain\\s*\\(\\s*SDL_Renderer\\s+\\*\\s+\\w+\\s*\\)\\s*\\{\\s*",
    std::regex_constants::ECMAScript
  )));

  std::getline(testOut, buf);
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "\\s*double\\s+\\w+X\\s*=\\s*\\w+\\/\\s*2\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));
  
  std::getline(testOut, buf);
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "\\s*double\\s+\\w+Y\\s*=\\s*\\w+\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));
  
  std::getline(testOut, buf);
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "\\s*double\\s+\\w+\\s*=\\s*\\d+\\s*\\*\\s*\\("
    "\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));

  for(int i = 0;std::getline(testOut, buf), i < 3; i++){
    std::cerr << buf << std::endl;
    ASSERT_TRUE(std::regex_match(buf, std::regex(
      "\\s*double\\s+\\w+\\s*=\\s*100\\s*;\\s*",
      std::regex_constants::ECMAScript
    ))); 
  }
  
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "\\s+SDL_SetRenderDrawColor\\s*\\("
    "\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,255\\s*\\)"
    "\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));

  std::getline(testOut, buf);
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "\\s*const\\s+double\\s+\\w+X\\s*=\\s*\\w+\\/\\s*2\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));
  
  std::getline(testOut, buf);
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "\\s*const\\s+double\\s+\\w+Y\\s*=\\s*\\w+\\/\\s*2\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));

  std::getline(testOut, buf);
  std::cerr << buf << std::endl;
  ASSERT_TRUE(std::regex_match(buf, std::regex(
    "\\s*}\\s*",
    std::regex_constants::ECMAScript
  )));
}

TEST(CodeGeneratorTestSuite, AddMain){
  std::stringstream in1;
  
  std::stringstream stream;
  stream 
    << "begin\n"
    << "end\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  auto in2 = parser.file();
  CodeGenerator test(in1, in2);
  ASSERT_EXIT((test.AddMain(), exit(0)), ::testing::ExitedWithCode(0), ".*");
}

TEST(CodeGeneratorTestSuite, BasicEmptyMain){
  const char * testFile = "EmptyMainTest.out";
  std::filesystem::remove(testFile);

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

  /*std::regex reg("(?:.|\\n)*(?:#include\\s+<\\w*\\.h>(?:.|\\n)*){2,}(?:.|\\n)*int main\\(int argc, const char \\*argv\\[\\]\\)\\s*\\{(?:.|\\n)*");
  EXPECT_TRUE(std::regex_match())
*/
  test.output.flush();
  std::istream &toTest(interface.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  interface.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));
}

TEST(CodeGeneratorTestSuite, AddFunctionDeclaration){
  std::stringstream in1;
  
  std::stringstream stream;
  stream 
    << "begin\n"
    << "end\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  auto in2 = parser.file();
  CodeGenerator test(in1, in2);

  ASSERT_EXIT((test.AddFunctionDeclaration(), exit(0)), ::testing::ExitedWithCode(0), ".*");

  in1.clear();
  test.AddFunctionDeclaration();

  std::cerr << in1.str() << std::endl;

  ASSERT_TRUE(in1.str().length());
  std::string buf;
  std::getline(in1, buf);
  ASSERT_EQ(buf[0], '/');
  ASSERT_EQ(buf[1], '/');

  std::regex testReg("void TurtelMain(.+);.*");
  std::getline(in1, buf);
  std::smatch match;
  ASSERT_TRUE(std::regex_match(buf, match, testReg));
  
}

TEST(CodeGeneratorTestSuite, BasicWalk){
  const char *testFile = "BasicWalkTest.out";
  std::filesystem::remove(testFile);
  LLVMInterface interface(testFile);
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  walk 50\n"
    << "end\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.file();

  EXPECT_TRUE(astStart);
  EXPECT_TRUE(astStart->main());
  EXPECT_EQ(astStart->calcdef().size(),0);
  EXPECT_EQ(astStart->pathdef().size(),0);
  EXPECT_FALSE(astStart->main()->isEmpty());
  EXPECT_EQ(astStart->main()->stat().size(), 1);

  CodeGenerator test(interface.llvmFile, astStart);
  ASSERT_EQ(test.astMain, astStart->main());
  test.GenerateCode();
  
  interface.llvmFile.seekg(0);
  std::regex checkForDraw(
          "\\s+SDL_RenderDrawLine\\s*\\("
          "\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*\\"
          ")\\s*;\\s*");

  std::string line;
  int ret = 0;
  while (std::getline(interface.llvmFile, line)) {
    std::cerr << line ;
    if(std::regex_match(line, checkForDraw)){
      ret++;
      std::cerr << "//found";
    }
    std::cerr << std::endl;
  }

  interface.llvmFile.seekg(0);
  
  ASSERT_EQ(ret, 1);

  interface.CallLLVM();
  ASSERT_TRUE(std::filesystem::exists(testFile));

}

TEST(CodeGeneratorTestSuite, BasicJump){
  const char *testFile = "BasicWalkTest.out";
  std::filesystem::remove(testFile);
  LLVMInterface interface(testFile);
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  jump 50\n"
    << "  walk 50\n"
    << "end\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.file();

  EXPECT_TRUE(astStart);
  EXPECT_TRUE(astStart->main());
  EXPECT_EQ(astStart->calcdef().size(),0);
  EXPECT_EQ(astStart->pathdef().size(),0);
  EXPECT_FALSE(astStart->main()->isEmpty());
  EXPECT_EQ(astStart->main()->stat().size(), 2);

  CodeGenerator test(interface.llvmFile, astStart);
  test.GenerateCode();
  
  interface.llvmFile.seekg(0);
  std::regex checkForDraw(
          "\\s+SDL_RenderDrawLine\\s*\\("
          "\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*\\"
          ")\\s*;\\s*");

  std::string line;
  int ret = 0;
  while (std::getline(interface.llvmFile, line)) {
    std::cerr << line ;
    if(std::regex_match(line, checkForDraw)){
      ret++;
      std::cerr << "//found";
    }
    std::cerr << std::endl;
  }

  interface.llvmFile.seekg(0);
  
  ASSERT_EQ(ret, 1);

  interface.CallLLVM();
  ASSERT_TRUE(std::filesystem::exists(testFile));
}

/*
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
