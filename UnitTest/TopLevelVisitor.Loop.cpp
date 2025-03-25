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

TEST_F(TopLevelVisitorTest, WhileOptimizeTrue){
  inputStream
    << "while 5 = 5 do\n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->while_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  TopLevelVisitorTest::SetInfLoopFlag(1);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*while\\s*\\(\\s*1\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, WhileOptimizeFalse){
  inputStream
    << "while 5 <> 5 do\n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->while_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  TopLevelVisitorTest::SetInfLoopFlag(1);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, WhileNoOptimize){
  inputStream
    << "while 5 = zwi do\n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->while_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  TopLevelVisitorTest::SetInfLoopFlag(1);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*while\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, DoUntilOptimizeTrue){
  inputStream
    << "repeat\n"
    << "  store 5 in zwi\n"
    << "untile 5 <> 5\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->doUntil();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  TopLevelVisitorTest::SetInfLoopFlag(1);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*do\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\}\\s*while\\s*\\(\\s*1\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, DoUntilOptimizeFalse){
  inputStream
    << "repeat\n"
    << "  store 5 in zwi\n"
    << "untile 5 = 5\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->doUntil();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  TopLevelVisitorTest::SetInfLoopFlag(1);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchKomment);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, DoUntilNoOptimize){
  inputStream
    << "repeat\n"
    << "  store 5 in zwi\n"
    << "untile 5 = zwi\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  //pars the test
  auto astStart = parser->doUntil();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  TopLevelVisitorTest::SetInfLoopFlag(1);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*do\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\}\\s*while\\s*\\(\\s*!\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\)\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, ToForInteger){
  inputStream
    << "do 5 times \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->toFor();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*size_t\\s*i\\s*=\\s*0\\s*;\\s*i\\s*<\\s*5\\s*;\\s*\\+\\+i\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, ToForDecimal){
  inputStream
    << "do 5.5 times \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->toFor();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*size_t\\s*i\\s*=\\s*0\\s*;\\s*i\\s*<\\s*6\\s*;\\s*\\+\\+i\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, ToForVariable){
  inputStream
    << "do zwi times \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->toFor();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*size_t\\s*i\\s*=\\s*0\\s*;\\s*i\\s*<\\s*\\w+\\s*;\\s*\\+\\+i\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, SimplUpForIntInt){
  inputStream
    << "counter i from 0 to 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*<\\s*5\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplUpForDoubleInt){
  inputStream
    << "counter i from 0.1 to 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*<\\s*5\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplUpForIntDouble){
  inputStream
    << "counter i from 0 to 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*<\\s*5.5\\d*\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplUpForDoubleDouble){
  inputStream
    << "counter i from 0.1 to 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*<\\s*5.5\\d*\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplUpForVarInt){
  inputStream
    << "counter i from zwi to 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;\\s*\\w+\\s*<\\s*5\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, SimplDownForIntInt){
  inputStream
    << "counter i from 0 downto 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplDownForDoubleInt){
  inputStream
    << "counter i from 0.1 downto 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplDownForIntDouble){
  inputStream
    << "counter i from 0 downto 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*>\\s*5.5\\d*\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplDownForDoubleDouble){
  inputStream
    << "counter i from 0.1 downto 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*>\\s*5.5\\d*\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, SimplDownForVarInt){
  inputStream
    << "counter i from zwi downto 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, StepUpForIntInt){
  inputStream
    << "counter i from 0 to 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0\\s*;"
          "\\s*\\w+\\s*<\\s*5\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepUpForDoubleInt){
  inputStream
    << "counter i from 0.1 to 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0.1\\d*\\s*;"
          "\\s*\\w+\\s*<\\s*5\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          ));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepUpForIntDouble){
  inputStream
    << "counter i from 0 to 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0\\s*;"
          "\\s*\\w+\\s*<\\s*5.5\\d*\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          ));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepUpForDoubleDouble){
  inputStream
    << "counter i from 0.1 to 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0.1\\d*\\s*;"
          "\\s*\\w+\\s*<\\s*5.5\\d*\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          ));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepUpForVarInt){
  inputStream
    << "counter i from zwi to 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;"
          "\\s*\\w+\\s*<\\s*5\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          ));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST_F(TopLevelVisitorTest, StepDonwForIntInt){
  inputStream
    << "counter i from 0 downto 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex( "\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*__usr_\\w+\\s*-=\\s*2\\s*\\)\\s*\\{\\s*$"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepDownForDoubleInt){
  inputStream
    << "counter i from 0.1 downto 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*\\d+\\.\\d+\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*__usr_\\w+\\s*-=\\s*2\\s*\\)\\s*\\{\\s*$"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepDonwForIntDouble){
  inputStream
    << "counter i from 0 downto 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*>\\s*5\\.5\\d*\\s*;\\s*__usr_\\w+\\s*-=\\s*2\\s*\\)\\s*\\{\\s*$"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepDownForDoubleDouble){
  inputStream
    << "counter i from 0.1 downto 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, matchPragmaUnrolle);
  
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*\\d+\\.\\d+\\s*;\\s*\\w+\\s*>\\s*5\\.5\\d*\\s*;\\s*__usr_\\w+\\s*-=\\s*2\\s*\\)\\s*\\{\\s*$"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
TEST_F(TopLevelVisitorTest, StepDonwForVarInt){
  inputStream
    << "counter i from zwi downto 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ;

  TopLevelVisitorTest::SetupParser();

  auto astStart = parser->for_();
  ASSERT_TRUE(astStart);

  TopLevelVisitorTest::SetVariables(astStart);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  ASSERT_REGEX(line, std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*__usr_\\w+\\s*-=\\s*2\\s*\\)\\s*\\{\\s*$"));

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchAssigne5);

  std::getline(retStream, line);
  ASSERT_REGEX(line, matchClosingCrlBracket);

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}
