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

TEST_F(TopLevelVisitorTest, StopNOK){
  inputStream
    << "stop 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->stop();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__envfunc_stop\\s*\\(\\s*5\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StopImplizitOK){
  inputStream
    << "stop"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->stop();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__envfunc_stop\\s*\\(\\s*0\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StopExplizitOK){
  inputStream
    << "stop 0"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->stop();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__envfunc_stop\\s*\\(\\s*0\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, FinishNOK){
  inputStream
    << "finish 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->finish();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__envfunc_fin\\s*\\(\\s*5\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, FinishImplizitOK){
  inputStream
    << "finish"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->finish();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__envfunc_fin\\s*\\(\\s*0\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, FinishExplizitOK){
  inputStream
    << "finish 0"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->finish();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__envfunc_fin\\s*\\(\\s*0\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}


