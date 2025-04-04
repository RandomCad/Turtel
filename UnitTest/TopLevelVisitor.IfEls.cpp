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

TEST_F(TopLevelVisitorTest, SingleIfOptimizeTrue){
  inputStream
    << "if 5 = 5 then\n"
    << "  store 5 in zwi\n"
    << "endif\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();
  
  //pars the test
  auto astStart = parser->if_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);

  std::any ret = astStart->accept(&toTest);
  
  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SingleIfOptimizeFalse){
  inputStream
    << "if 5 <> 5 then\n"
    << "  store 5 in zwi\n"
    << "endif\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->if_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);

  std::any ret = astStart->accept(&toTest);
  
  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  //TODO add this lines to most of the other tests!
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SingleIfNoOptimize){
  inputStream
    << "if 5 = zwi then\n"
    << "  store 5 in zwi\n"
    << "endif\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->if_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);

  std::any ret = astStart->accept(&toTest);
  
  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*if\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\{\\s*"));
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, ElseIfOptimizeTrue){
  inputStream
    << "if 5 = 5 then\n"
    << "  store 5 in zwi\n"
    << "else\n"
    << "  store -5 in zwi\n"
    << "endif\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->if_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);

  std::any ret = astStart->accept(&toTest);
  
  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, ElseIfOptimizeFalse){
  inputStream
    << "if 5 <> 5 then\n"
    << "  store 5 in zwi\n"
    << "else\n"
    << "  store -5 in zwi\n"
    << "endif\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->if_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*-5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, ElseIfNoOptimize){
  inputStream
    << "if 5 = zwi then\n"
    << "  store 5 in zwi\n"
    << "else\n"
    << "  store -5 in zwi\n"
    << "endif\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->if_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*if\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*else\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*-5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}


