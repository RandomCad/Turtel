#include "SceneParser.h"
#include "src/Function.h"
#include "src/Variable.h"
#include "UnitTest/TestHelper.h"
#include "gtest/gtest.h"
#include <regex>
#include <tree/ErrorNode.h>

#define ASSERT_REGEX(line, Regex) \
  std::cerr << line << std::endl;\
  ASSERT_TRUE(\
      std::regex_match(\
        line,\
        Regex\
        )\
      );

TEST(Function, GetSimplPathDeclaration){
  std::stringstream mem;
  mem << "path test1 endpath\n"
      << "path test2 (var1) endpath\n"
      << "path test3 (var2) walk 5 endpath\n"
      ;
  getParser(mem);

  ///get the first pathdef
  {
    auto st = parser.pathdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::VOID, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        std::regex("\\s*void\\s*\\w+\\s*\\(\\s*\\)\\s*;\\s*")
        );
  }
  std::regex declWithOneVar("\\s*void\\s*\\w+\\s*\\(\\s*\\w+\\s+__usr_func\\w+\\s*\\)\\s*;\\s*");
  ///get the first pathdef
  {
    auto st = parser.pathdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::VOID, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
  ///get the first pathdef
  {
    auto st = parser.pathdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::VOID, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
}

TEST(Function, GetSimplCalcDeclaration){
  std::stringstream mem;
  ///the paramlist isn't optional
  mem << "calculation test1 () returns 5 endcalc\n"
      << "calculation test2 (var1) returns 5 endcalc\n"
      << "calculation test3 (var2) walk 5 returns 5 endcalc\n"
      ;
  getParser(mem);

  ///get the first pathdef
  {
    auto st = parser.calcdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::DOUBLE, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        std::regex("\\s*double\\s*\\w+\\s*\\(\\s*\\)\\s*;\\s*")
        );
  }
  std::regex declWithOneVar("\\s*double\\s*\\w+\\s*\\(\\s*\\w+\\s+__usr_func\\w+\\s*\\)\\s*;\\s*");
  ///get the first pathdef
  {
    auto st = parser.calcdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::DOUBLE, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
  ///get the first pathdef
  {
    auto st = parser.calcdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::DOUBLE, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
}

TEST(Function, GetMainDeclaration){
  std::stringstream mem;
  ///the paramlist isn't optional
  mem << "begin end\n"
      ;
  getParser(mem);

  ///get the first pathdef
  {
    auto st = parser.main();
    ASSERT_TRUE(st);
    Function test("test", VarType::RENDERER, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        std::regex("\\s*SDL_Renderer\\s+\\*\\s*\\w+\\s*\\(\\s*\\w+\\s+\\*\\s*\\w+\\s*\\)\\s*;\\s*")
        );
  }
}

TEST(Function, GetSimplPathCall){
  std::stringstream mem;
  mem << "path test1 endpath\n"
      << "path test2 (var1) endpath\n"
      << "path test3 (var2) walk 5 endpath\n"
      ;
  getParser(mem);

  ///get the first pathdef
  {
    auto st = parser.pathdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::VOID, st);
    ASSERT_REGEX(
        test.getFunctionCall(std::vector<Variable>()),
        std::regex("\\s*\\w+\\s*\\(\\s*\\)\\s*\\s*")
        );
  }
  std::regex declWithOneVar("\\s*void\\s*\\w+\\s*\\(\\s*\\w+\\s+__usr_func\\w+\\s*\\)\\s*;\\s*");
  ///get the first pathdef
  {
    auto st = parser.pathdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::VOID, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
  ///get the first pathdef
  {
    auto st = parser.pathdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::VOID, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
}

TEST(Function, GetSimplCalcCall){
  std::stringstream mem;
  ///the paramlist isn't optional
  mem << "calculation test1 () returns 5 endcalc\n"
      << "calculation test2 (var1) returns 5 endcalc\n"
      << "calculation test3 (var2) walk 5 returns 5 endcalc\n"
      ;
  getParser(mem);

  ///get the first pathdef
  {
    auto st = parser.calcdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::DOUBLE, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        std::regex("\\s*double\\s*\\w+\\s*\\(\\s*\\)\\s*;\\s*")
        );
  }
  std::regex declWithOneVar("\\s*double\\s*\\w+\\s*\\(\\s*\\w+\\s+__usr_func\\w+\\s*\\)\\s*;\\s*");
  ///get the first pathdef
  {
    auto st = parser.calcdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::DOUBLE, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
  ///get the first pathdef
  {
    auto st = parser.calcdef();
    ASSERT_TRUE(st);
    Function test("test", VarType::DOUBLE, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        declWithOneVar
        );
  }
}

TEST(Function, GetMainCall){
  std::stringstream mem;
  ///the paramlist isn't optional
  mem << "begin end\n"
      ;
  getParser(mem);

  ///get the first pathdef
  {
    auto st = parser.main();
    ASSERT_TRUE(st);
    Function test("test", VarType::RENDERER, st);
    ASSERT_REGEX(
        test.getFunctionDeclaration(),
        std::regex("\\s*SDL_Renderer\\s+\\*\\s*\\w+\\s*\\(\\s*\\w+\\s+\\*\\s*\\w+\\s*\\)\\s*;\\s*")
        );
  }
}

