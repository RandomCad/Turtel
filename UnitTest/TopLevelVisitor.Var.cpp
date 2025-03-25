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

TEST_F(TopLevelVisitorTest, MultVar){
  inputStream
    << "mul test by 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->multVar();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*\\*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, DivVar){
  inputStream
    << "div test by 5"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->divVar();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*/=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SubVar){
  inputStream
    << "sub 5 from test"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->subVar();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*-=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, AddVar){
  inputStream
    << "add 5 to test"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->addVar();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*__usr_\\w+\\s*\\+=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StorVar){
  inputStream
    << "store 5 in test"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->storeVar();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

