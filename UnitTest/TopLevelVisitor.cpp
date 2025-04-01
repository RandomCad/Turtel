#include "src/TopLevelVisitor.h"
#include "gtest/gtest.h"
#include <ANTLRInputStream.h>
#include <CommonTokenStream.h>
#include <ParserRuleContext.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <any>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <vector>

#include "UnitTest/TestHelper.h"
#include "UnitTest/TopLevelVisitor.h"
#include "libs/SceneParser.h"
#include "libs/SceneLexer.h"
#include "src/Variable.h"
#include "src/VariableVisitor.h"
#include "src/InternalVarNames.h"
#include "TestHelper.h"

std::regex matchKomment = std::regex("\\s*\\/\\/.*$");
std::regex matchPragmaUnrolle = std::regex("\\s*#pragma\\s+unroll\\s*");
std::regex matchClosingCrlBracket = std::regex("\\s*\\}\\s*");
std::regex matchFuncHead = std::regex("\\s*\\w+\\s+\\w+\\s*\\(\\s*(\\s*double\\s+__usr_\\w+(,\\s*double\\s+__usr_\\w+)*)?\\)\\s*\\{\\s*$");
std::regex matchAssigne5 = std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*$");

std::string getRandomID();

bool CheckSurfaceForBlack(SDL_Surface *a){
  for (Uint32 *i = (Uint32*)a->pixels ; i - (Uint32*)a->pixels < a->h *a->w; ++i) if(*i != 0xff000000) return false;
  return true;
}

void TopLevelVisitorTest::SetFunction(std::unordered_map<std::string, Function> &a){
  toTest.funcs = a;
}
TopLevelVisitorTest::TopLevelVisitorTest() :
  toTest(retStream)
{
  toTest.envVar = std::unordered_map<std::string, Variable>(ENV_VAR);
  int seed = time(00);
  RecordProperty("seed", seed);
  std::cout << "seed: " << seed << "  " << ::testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl; 
  srand(seed);
}
void TopLevelVisitorTest::SetupParser(){
  input = ANTLRInputStream(inputStream);
  lexer = new SceneLexer(&input);
  tokens = new CommonTokenStream(lexer);
  parser = new SceneParser(tokens);
}
TopLevelVisitorTest::~TopLevelVisitorTest() {
  delete lexer;
  delete tokens;
  delete parser;
}
void TopLevelVisitorTest::SetVariables(antlr4::ParserRuleContext *a){
  toTest.ctxVar = VarVisitor().getVariableContext(a);
}
void TopLevelVisitorTest::SetInfLoopFlag(int a){
  toTest.infinitLoopFlag = a;
}

using  namespace antlr4;

TEST(TopLevelVisitor, TestCircleTG){
  const char * testFile = "Circle.out";
  std::filesystem::remove(testFile);

  std::filebuf fb;
  if(!fb.open("./TestData/circle.tg", std::ios::in)){
    throw  "error"; //TODO;
  }

  std::istream stream(&fb);

  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.file();
  EXPECT_TRUE(astStart);
  EXPECT_TRUE(astStart->main());
  EXPECT_EQ(astStart->calcdef().size(), 0);
  EXPECT_EQ(astStart->pathdef().size(), 1);
  
  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  test.llvm.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  int exitCode = std::system((std::string("./") + testFile).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);
}
TEST(TopLevelVisitor, TestCalcDefCommand){
  const char * testFile = "TestCalcDefCommand.out";
  std::filesystem::remove(testFile);

  std::stringstream stream;
  stream 
    << "calculation TheNumber ()" << std::endl
    << "  store 5 in test\n"
    << "  store -3 in ret\n"
    << "  add ret to test\n"  //test = 2
    << "  mul ret by test\n"  //ret = -3 * 2 = -6
    << "  div test by ret\n"  //test = 2 / -6 = -0,5
    << "  store ret in test\n"//test = ret = -6
    << "  mul ret by test\n"  //ret = 36
    << "  returns ret" << std::endl //returns 36
    << "endcalc" << std::endl
    << "begin\n"
    << "  finish TheNumber()" << std::endl //output should be 36
    << "end\n"
    << std::endl;

  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.file();
  EXPECT_TRUE(astStart);
  EXPECT_TRUE(astStart->main());
  EXPECT_EQ(astStart->calcdef().size(), 1);
  EXPECT_EQ(astStart->pathdef().size(), 0);
  
  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  test.llvm.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  int exitCode = std::system((std::string("./") + testFile).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 36);
}
TEST(TopLevelVisitor, TestCalcDefRecursiv){
  const char * testFile = "TestCalcDefCommand.out";
  std::filesystem::remove(testFile);

  std::stringstream stream;
  stream 
    << "calculation fib (f)" << std::endl
    << "  if f <= 1 then" << std::endl
    << "    store 1 in ret" << std::endl
    << "  else" << std::endl
    << "    store fib(f - 1) + fib(f - 2) in ret" << std::endl
    << "  endif"
    << "  returns ret" << std::endl 
    << "endcalc" << std::endl
    << "begin\n"
    << "  finish fib(@0)" << std::endl 
    << "end\n"
    << std::endl;

  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.file();
  EXPECT_TRUE(astStart);
  EXPECT_TRUE(astStart->main());
  EXPECT_EQ(astStart->calcdef().size(), 1);
  EXPECT_EQ(astStart->pathdef().size(), 0);
  
  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  test.llvm.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  int exitCode = std::system((std::string("./") + testFile + " 1").c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 1);

  exitCode = std::system((std::string("./") + testFile + " 2").c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 2);
}

TEST(TopLevelVisitor, TestVarCommands){
  const char * testFile = "TestVarCommand.out";
  std::filesystem::remove(testFile);

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
  
  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  test.llvm.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  int exitCode = std::system((std::string("./") + testFile).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 36);
}

TEST(TopLevelVisitor, TestTrivialSave){
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
  
  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  test.llvm.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  int exitCode = std::system((std::string("./") + testFile).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);
  ASSERT_TRUE(std::filesystem::exists("test.png"));

  SDL_Surface *a = IMG_Load("./test.png");

  ASSERT_EQ(a->format->BytesPerPixel, 4);
  for (Uint32 *i = (Uint32*)a->pixels ; i - (Uint32*)a->pixels < a->h *a->w; ++i) {
    ASSERT_EQ(*i, 0xff000000) << (i - (Uint32*)a->pixels) % a->h << (i - (Uint32*)a->pixels)/a->w;
  }
}
TEST(TopLevelVisitor, TestDoubleSave){
  const char * testFile = "TestTrivialSave.out";
  std::string pngTest1 = "test";
  std::string pngTest2 = "test1";
  std::filesystem::remove(testFile);
  std::filesystem::remove(pngTest1);
  std::filesystem::remove(pngTest2);

  LLVMInterface interface(testFile);
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  save " << pngTest1 << "\n"
    << "  save " << pngTest2 << "\n"
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
  
  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  test.llvm.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));

  int exitCode = std::system((std::string("./") + testFile).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);

  {
    pngTest1 = std::string("./") + pngTest1 + std::string(".png");
    ASSERT_TRUE(std::filesystem::exists(pngTest1));

    SDL_Surface *a = IMG_Load(pngTest1.c_str());

    ASSERT_EQ(a->format->BytesPerPixel, 4);
    for (Uint32 *i = (Uint32*)a->pixels ; i - (Uint32*)a->pixels < a->h *a->w; ++i) {
      ASSERT_EQ(*i, 0xff000000) << (i - (Uint32*)a->pixels) % a->h << (i - (Uint32*)a->pixels)/a->w;
    }
  }
  {
    pngTest2 = std::string("./") + pngTest2 + std::string(".png");
    ASSERT_TRUE(std::filesystem::exists(pngTest2));

    SDL_Surface *a = IMG_Load(pngTest2.c_str());

    ASSERT_EQ(a->format->BytesPerPixel, 4);
    for (Uint32 *i = (Uint32*)a->pixels ; i - (Uint32*)a->pixels < a->h *a->w; ++i) {
      ASSERT_EQ(*i, 0xff000000) << (i - (Uint32*)a->pixels) % a->h << (i - (Uint32*)a->pixels)/a->w;
    }
  }
}
TEST(TopLevelVisitor, BasicWalkSave){
  const char *testFile = "BasicWalkSaveTest.out";
  std::string pngTest1 = "test2";
  std::filesystem::remove(testFile);
  std::filesystem::remove(pngTest1);

  const std::string baseFileName = "BasicWalk";
  const std::string picFileName = baseFileName + ".png";
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  walk 50\n"
    << "  save " << pngTest1 << "\n"
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
  EXPECT_EQ(astStart->main()->statList()->stat().size(), 2);

  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::regex checkForDraw(
          "\\s+SDL_RenderDrawLine\\s*\\("
          "\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*\\"
          ")\\s*;\\s*");
  std::string line;
  int ret = 0;
  while (std::getline(test.llvm.llvmFile, line)) {
    std::cerr << line ;
    if(std::regex_match(line, checkForDraw)){
      ret++;
      std::cerr << "//found";
    }
    std::cerr << std::endl;
  }

  test.llvm.llvmFile.seekg(0);
  
  ASSERT_EQ(ret, 1);

  test.llvm.CallLLVM();
  ASSERT_TRUE(std::filesystem::exists(testFile));

  int exitCode = std::system((std::string("./") + testFile).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);

  {
    pngTest1 = std::string("./") + pngTest1 + std::string(".png");
    ASSERT_TRUE(std::filesystem::exists(pngTest1));

    SDL_Surface *a = IMG_Load(pngTest1.c_str());

    ASSERT_EQ(a->format->BytesPerPixel, 4);
    ASSERT_FALSE(CheckSurfaceForBlack(a));
  }
}

TEST(TopLevelVisitor, BasicEmptyMain){
  const char * testFile = "EmptyMainTest.out";
  std::filesystem::remove(testFile);

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
  
  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::string line;
  while (std::getline(toTest, line)) {
    std::cerr << line << std::endl;
  }

  toTest.seekg(0);
  test.llvm.CallLLVM();
  
  ASSERT_TRUE(std::filesystem::exists(testFile));
}
TEST(TopLevelVisitor, BasicWalk){
  const char *testFile = "BasicWalkTest.out";
  std::filesystem::remove(testFile);

  const std::string baseFileName = "BasicWalk";
  const std::string picFileName = baseFileName + ".png";
  std::stringstream stream;
  stream 
    << "begin\n"
    << "  walk 50\n"
    //<< "  save " << baseFileName << "\n"
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
  EXPECT_EQ(astStart->main()->statList()->stat().size(), 1);

  TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::regex checkForDraw(
          "\\s+SDL_RenderDrawLine\\s*\\("
          "\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*\\"
          ")\\s*;\\s*");
  std::string line;
  int ret = 0;
  while (std::getline(test.llvm.llvmFile, line)) {
    std::cerr << line ;
    if(std::regex_match(line, checkForDraw)){
      ret++;
      std::cerr << "//found";
    }
    std::cerr << std::endl;
  }

  test.llvm.llvmFile.seekg(0);
  
  ASSERT_EQ(ret, 1);

  test.llvm.CallLLVM();
  ASSERT_TRUE(std::filesystem::exists(testFile));

  return;

  ASSERT_TRUE(std::filesystem::exists(picFileName));

  SDL_Surface *a = IMG_Load(picFileName.c_str());

  ASSERT_EQ(a->format->BytesPerPixel, 4);
  ASSERT_FALSE(CheckSurfaceForBlack(a));
}
TEST(TopLevelVisitor, BasicJump){
  const char *testFile = "BasicWalkTest.out";
  std::filesystem::remove(testFile);

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
  EXPECT_EQ(astStart->main()->statList()->stat().size(), 2);

TopLevelVisitor test(testFile);
  test.visitFile(astStart);

  std::istream &toTest(test.llvm.llvmFile);

  toTest.seekg(0);

  std::regex checkForDraw(
          "\\s+SDL_RenderDrawLine\\s*\\("
          "\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*\\"
          ")\\s*;\\s*");
  std::string line;
  int ret = 0;
  while (std::getline(test.llvm.llvmFile, line)) {
    std::cerr << line ;
    if(std::regex_match(line, checkForDraw)){
      ret++;
      std::cerr << "//found";
    }
    std::cerr << std::endl;
  }

  test.llvm.llvmFile.seekg(0);
  
  ASSERT_EQ(ret, 1);

  test.llvm.CallLLVM();
  ASSERT_TRUE(std::filesystem::exists(testFile));
}

TEST_F(TopLevelVisitorTest, CalcDef){
  inputStream
    << "calculation TheNumber ()" << std::endl
    << "  store 5 in test" << std::endl
    << "  store -3 in ret" << std::endl
    << "  add ret to test" << std::endl  //test = 2
    << "  mul ret by test" << std::endl  //ret = -3 * 2 = -6
    << "  div test by ret" << std::endl  //test = 2 / -6 = -0,5
    << "  store ret in test" << std::endl//test = ret = -6
    << "  mul ret by test" << std::endl  //ret = 36
    << "  returns ret" << std::endl //returns 36
    << "endcalc" << std::endl
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->calcdef();
  TopLevelVisitorTest::SetFunction({{"TheNumber", Function("TheNumber", VarType::DOUBLE, astStart)}});

  ASSERT_TRUE(astStart);

  {
    astStart->accept(&toTest);

    std::regex assigne("\\s*__usr_\\w+\\s*=\\s*(-?\\d+|__usr_\\w+)\\s*;\\s*$");
    std::regex calcAssigne("\\s*__usr_\\w+\\s*(\\+=|-=|\\*=|/=)\\s*__usr\\w+\\s*;\\s*$");

    std::string line;
    std::getline(retStream, line);
    ASSERT_REGEX(line, matchFuncHead);

    std::getline(retStream, line);
    ASSERT_REGEX(line, std::regex("\\s*double\\s+__usr_\\w+\\s*=\\s*0\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_REGEX(line, std::regex("\\s*double\\s+__usr_\\w+\\s*=\\s*0\\s*;\\s*$"));

    for (int i = 0; i < 2; ++i){
      std::getline(retStream, line);
      ASSERT_FALSE(retStream.eof());
      ASSERT_REGEX(line, assigne);
    }

    for (int i = 0; i < 3; ++i){
      std::getline(retStream, line);
      ASSERT_FALSE(retStream.eof());
      ASSERT_REGEX(line, calcAssigne);
    }

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, assigne);

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, calcAssigne);

    //return
    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, std::regex("\\s*return\\s+__usr_\\w+\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, matchClosingCrlBracket);

    std::getline(retStream, line);
    ASSERT_TRUE(retStream.eof());
  }
}
TEST_F(TopLevelVisitorTest, PathDef){
  inputStream
    << "path circle(r,n)\n"
    << "  jump r\n"
    << "  store 180-360/n in beta\n"
    << "  store 2*(2*@pi*r/2)/n in a\n"
    << "  turn right 180-beta/2\n"
    << "  counter x from 0 to n do\n"
    << "    walk a\n"
    << "    turn right 180-beta\n"
    << "  done\n"
    << "endpath\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->pathdef();
  TopLevelVisitorTest::SetFunction({{"circle", Function("circle", VarType::DOUBLE, astStart)}});

  ASSERT_TRUE(astStart);

  {
    astStart->accept(&toTest);

    std::regex assigne("\\s*__usr_\\w+\\s*=\\s*(-?\\d+|__usr_\\w+)\\s*;\\s*$");
    std::regex calcAssigne("\\s*__usr_\\w+\\s*(\\+=|-=|\\*=|/=)\\s*__usr\\w+\\s*;\\s*$");
    std::regex moveX("\\s*__env_posX\\s*=\\s*__env_posX\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\(__env_rot\\)\\s*;");
    std::regex moveY("\\s*__env_posY\\s*=\\s*__env_posY\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\(__env_rot\\)\\s*;");

    std::string line;
    std::getline(retStream, line);
    ASSERT_REGEX(line, matchFuncHead);

    for (int i = 0; i < 5; i++){
      std::getline(retStream, line);
      ASSERT_REGEX(line, std::regex("\\s*double\\s+__usr_\\w+\\s*=\\s*0\\s*;\\s*$"));
    }

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, moveX);

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, moveY);

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, std::regex("\\s*__usr_\\w+\\s*=\\s*\\d+\\s*-\\s*\\d+\\s*\\/\\s*__usr_\\w+\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, std::regex("\\s*__usr_\\w+\\s*=\\s*\\d+\\s*\\*\\s*\\(\\s*\\d+\\.\\d+\\s*\\*\\s*__usr_\\w+\\s*\\/\\s*\\d+\\s*\\)\\s*\\/\\s*__usr_\\w+\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, std::regex("\\s*__env_rot\\s*\\+=\\s*\\(\\s*\\d+\\s*\\-\\s*__usr_\\w+\\s*\\/\\s*\\d+\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*__usr_\\w+\\s*=\\s*\\d+\\s*;\\s*__usr_\\w+\\s*<\\s*__usr_\\w+\\s*;\\s*\\+\\+__usr_\\w+\\s*\\)\\s*\\{\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, std::regex("\\s*SDL_RenderDrawLine\\s*\\(\\s*__env_rnd\\s*,\\s*__env_posX\\s*,\\s*__env_posY\\s*,\\s*__env_posX\\s*\\+\\s*__usr_\\w+\\s*\\*\\s*cos\\s*\\(\\s*__env_rot\\s*\\)\\s*,\\s*__env_posY\\s*\\+\\s*__usr_a\\s*\\*\\s*sin\\s*\\(\\s*__env_rot\\s*\\)\\s*\\)\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, moveX);

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, moveY);
    
    std::getline(retStream, line);
    ASSERT_REGEX(line, std::regex("\\s*SDL_SetRenderTarget\\s*\\(\\s*__env_\\w+\\s*,\\s*NULL\\s*\\)\\s*;\\s*$"));
    std::getline(retStream, line);
    ASSERT_REGEX(line, std::regex("\\s*SDL_RenderClear\\s*\\(\\s*__env_\\w+\\s*\\)\\s*;\\s*$"));
    std::getline(retStream, line);
    ASSERT_REGEX(line, std::regex("\\s*SDL_RenderCopy\\s*\\(\\s*__env_\\w+\\s*,\\s*__env_\\w+\\s*,\\s*NULL\\s*,\\s*NULL\\s*\\)\\s*;\\s*$"));
    std::getline(retStream, line);
    ASSERT_REGEX(line, std::regex("\\s*SDL_RenderPresent\\s*\\(\\s*__env_\\w+\\s*\\)\\s*;\\s*$"));
    std::getline(retStream, line);
    ASSERT_REGEX(line, std::regex("\\s*SDL_SetRenderTarget\\s*\\(\\s*__env_\\w+\\s*,\\s*__env_\\w+\\s*\\)\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, std::regex("\\s*__env_rot\\s*\\+=\\s*\\(\\s*\\d+\\s*\\-\\s*__usr_\\w+\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*$"));

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, matchClosingCrlBracket);

    std::getline(retStream, line);
    ASSERT_FALSE(retStream.eof());
    ASSERT_REGEX(line, matchClosingCrlBracket);

    std::getline(retStream, line);
    ASSERT_TRUE(retStream.eof());
  }
}
TEST_F(TopLevelVisitorTest, ParamList){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    size_t am = rand() % TEST_AMMOUNT;
    std::vector<std::string> IDs(am);
    for(size_t i = 0; i < am; ++i) IDs[i] = getRandomID(); 
    inputStream << '(';
    for(size_t i = 0; i < am - 1; ++i) inputStream << IDs[i] << ',' ; 
    inputStream << IDs[am-1] << ')';

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->paramlist();
    ASSERT_TRUE(astStart);
    TopLevelVisitorTest::SetVariables(astStart);
    ASSERT_EQ(astStart->var().size(), am);

    auto ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::vector<Variable>));
    std::vector<Variable> retVect = std::any_cast<std::vector<Variable>>(ret);
    
    ASSERT_EQ(retVect.size(), am);
  }
}

TEST_F(TopLevelVisitorTest, Clear){
  inputStream
    << "clear"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->clear();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*SDL_RenderClear\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, Color){
  inputStream
    << "color 255 0 128"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->colorCmd();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*SDL_SetRenderDrawColor\\s*\\(\\s*\\w+\\s*,\\s*255\\s*,\\s*0\\s*,\\s*128\\s*,\\s*255\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*255\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*0\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*128\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, Save){
  inputStream
    << "save test"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->save();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*save_texture\\s*\\(\\s*\".*\"\\s*,\\s*__env_\\w+\\s*,\\s*__env_\\w+\\s*\\)\\s*;\\s*$"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

#if FALSE
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, AcceptNumContext){
  std::stringstream stream;
  stream 
    << "5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.number();
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(dynamic_cast<SceneParser::NumberContext*>(astStart));
  ASSERT_TRUE(dynamic_cast<SceneParser::IntContext*>(astStart));

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);
  ASSERT_EQ(std::any_cast<int64_t>(ret), 5);
}

// Test für den "mark"-Befehl
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, MarkCommand) {
  std::stringstream stream;
  stream << "mark" << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.mark();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << "MarkCommand output: " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*pushMarker\\(\\(Marker\\)\\{\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*\\}\\);\\s*")
  ));
}

// Test für den "walk mark"-Befehl
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, WalkMarkCommand) {
  std::stringstream stream;
  stream << "walk mark" << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.walkMark();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  

  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 1): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*if \\(markerStackTop >= 0\\) \\{\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 2): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*Marker m = popMarker\\(\\);\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 3): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*SDL_RenderDrawLine\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*m\\.posX,\\s*m\\.posY\\s*\\)\\s*;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 4): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posX;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 5): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posY;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 6): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.rotation;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 7): " << line << std::endl;

  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\}\\s*(else\\s*\\{)?\\s*")
  ));
}

// Test für den "jump mark"-Befehl
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, JumpMarkCommand) {
  std::stringstream stream;
  stream << "jump mark" << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.jumpMark();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 1): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*if \\(markerStackTop >= 0\\) \\{\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 2): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*Marker m = popMarker\\(\\);\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 3): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posX;\\s*")
  ));

  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 4): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posY;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 5): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.rotation;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 6): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\}\\s*(else\\s*\\{)?\\s*")
  ));
}



#define BoolRetIs(ASSERT) \
  {\
    TopLevelVisitor visitor(zwi, var);\
    auto test = parser.cond();\
    ASSERT_TRUE(test);\
    std::any ret = test->accept(&visitor);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT(std::any_cast<bool>(ret));\
  }
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, LesEquThan){
  std::stringstream stream;
  stream
    << "5 <= 3\n"
    << "3 <= 5\n"
    << "3 <= 3\n"
    << "var <= 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*<=\\s*3\\s*"
            )
          )
        );
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, LesThan){
  std::stringstream stream;
  stream
    << "5 < 3\n"
    << "3 < 5\n"
    << "3 < 3\n"
    << "var < 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*<\\s*3\\s*"
            )
          )
        );
  }
} 

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, GreaterEquThan){
  std::stringstream stream;
  stream
    << "5 >= 3\n"
    << "3 >= 5\n"
    << "3 >= 3\n"
    << "var >= 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  { 
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>=\\s*3\\s*"
            )
          )
        );
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, GreaterThan){
  std::stringstream stream;
  stream
    << "3 > 5\n"
    << "5 > 3\n"
    << "3 > 3\n"
    << "var > 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*3\\s*"
            )
          )
        );
  }
} 

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, CondEqual){
  std::stringstream stream;
  stream
    << "3 = 5\n"
    << "5 = 3\n"
    << "3 = 3\n"
    << "var = 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*==\\s*3\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, CondUnequal){
  std::stringstream stream;
  stream
    << "3 <> 5\n"
    << "5 <> 3\n"
    << "3 <> 3\n"
    << "var <> 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*!=\\s*3\\s*"
            )
          )
        );
  }
} 

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, ClamCond){
  std::stringstream stream;
  stream
    << "(3 > 5)\n"
    << "(5 > 3)\n"
    << "(3 > 3)\n"
    << "(var > 3)\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\(\\s*\\w+\\s*>\\s*3\\s*\\)\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, NotCond){
  std::stringstream stream;
  stream
    << "NOT 3 > 5\n"
    << "NOT 5 > 3\n"
    << "NOT 3 > 3\n"
    << "NOT var > 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*!\\(\\s*\\w+\\s*>\\s*3\\s*\\)\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, AndCond){
  std::stringstream stream;
  stream
    << "3 > 5 AND 12 > 12.5\n"
    << "3 > 5 AND 13 > 12.5\n"
    << "5 > 3 AND -3 > 0\n"
    << "5 > 3 AND -3 > -5\n"
    << "var > 3 AND var < 100\n"
    << "var > 3 AND 100 < 100\n"
    << "var > 3 AND 10 < 100\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_TRUE);
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*&&\\s*\\w+\\s*<\\s*\\d+\\s*"
            )
          )
        );
  }
  BoolRetIs(ASSERT_FALSE);
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, OrCond){
  std::stringstream stream;
  stream
    << "3 > 5 OR 12 > 12.5\n"
    << "3 > 5 OR 13 > 12.5\n"
    << "5 > 3 OR -3 > 0\n"
    << "5 > 3 OR -3 > -5\n"
    << "var > 3 OR var < 100\n"
    << "var > 3 OR 100 < 100\n"
    << "var > 3 OR 10 < 100\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_TRUE);
  BoolRetIs(ASSERT_TRUE);
  BoolRetIs(ASSERT_TRUE);
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*\\|\\|\\s*\\w+\\s*<\\s*\\d+\\s*"
            )
          )
        );
  }
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*"
            )
          )
        );
  }
  BoolRetIs(ASSERT_TRUE);
} 
#undef BoolRetIs

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, NumExpr){
  std::stringstream stream;
  stream << "794651" << std::endl << "91231" << std::endl << "6541.89645" << std::endl
    << "786451.168754" << std::endl << ".8765413" << std::endl 
    << ".876541132" << std::endl;

  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  TopLevelVisitor visitor(zwi, var);

  auto test = parser.number();
  ASSERT_TRUE(test);
  int testNum = std::any_cast<int64_t>(test->accept(&visitor));
  ASSERT_EQ(testNum, 794651);

  test = parser.number();
  ASSERT_TRUE(test);
  testNum = std::any_cast<int64_t>(test->accept(&visitor));
  ASSERT_EQ(testNum, 91231);

  test = parser.number();
  ASSERT_TRUE(test);
  double testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, 6541.89645);

  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, 786451.168754);
  
  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, .8765413);

  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, .876541132);
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, NegExpr){
  std::stringstream stream;
  stream
    << -5
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  TopLevelVisitor visitor(zwi, var);

  auto test = parser.expr();

  ASSERT_TRUE(test);
  SceneParser::NegateContext *neg;
  ASSERT_TRUE(neg = dynamic_cast<SceneParser::NegateContext*>(test));
  ASSERT_EQ(neg->children.size(), 2);
  ASSERT_TRUE(neg->children[0]);
  ASSERT_STREQ(neg->children[0]->getText().c_str(), "-");
  ASSERT_TRUE(neg->children[1]);
  ASSERT_TRUE(dynamic_cast<SceneParser::NumberContext*>(neg->children[1]));
}

#endif
