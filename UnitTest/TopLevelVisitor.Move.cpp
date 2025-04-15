#include "src/TopLevelVisitor.h"
#include "gtest/gtest.h"
#include <ANTLRInputStream.h>
#include <CommonTokenStream.h>
#include <ParserRuleContext.h>
#include <any>
#include <cstdlib>
#include <ctime>
#include <regex>
#include <sstream>
#include <string>
#include <cmath>

#include "UnitTest/TestHelper.h"
#include "UnitTest/TopLevelVisitor.h"
#include "libs/SceneParser.h"
#include "src/VariableVisitor.h"
#include "TestHelper.h"

extern std::regex matchKomment ;
extern std::regex matchPragmaUnrolle;
extern std::regex matchClosingCrlBracket;
extern std::regex matchFuncHead;
extern std::regex matchAssigne5;
extern std::regex checkForDraw;

TEST_F(TopLevelVisitorTest, TurnExplizitRight){
  inputStream
    << "turn right 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->turnRight();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*\\+=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, TurnImplizitRight){
  inputStream
    << "turn 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->turnRight();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*\\+=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, TurnLeft){
  inputStream
    << "turn left 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->turnLeft();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*\\-=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, Direction){
  inputStream
    << "direction 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->direction();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*\\=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, JumpHome){
  inputStream
    << "jump home"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->jumpHome();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s+\\/\\s+2;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s+\\/\\s+2;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, WalkHome){
  inputStream
    << "walk home"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->walkHome();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*SDL_RenderDrawLine\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s+\\/\\s+2\\s*,\\s*\\w+\\s+\\/\\s+2\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s+\\/\\s+2;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s+\\/\\s+2;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, WalkFront){
  inputStream
    << "walk 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->walk();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, checkForDraw);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"));

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
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, WalkBack){
  inputStream
    << "walk back 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->walk();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, checkForDraw);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*cos\\(\\s*\\w+\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*sin\\(\\s*\\w+\\s*\\)\\s*;\\s*"));

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
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, JumpFront){
  inputStream
    << "jump 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->jump();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__env_posX\\s*=\\s*__env_posX\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\(__env_rot\\)\\s*;"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__env_posY\\s*=\\s*__env_posY\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\(__env_rot\\)\\s*;"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, JumpBack){
  inputStream
    << "jump back 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->jump();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*cos\\(\\s*\\w+\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*sin\\(\\s*\\w+\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}


