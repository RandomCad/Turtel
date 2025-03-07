#include "SceneParser.h"
#include "build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "UnitTest/TestHelper.h"
#include "gtest/gtest.h"
#include "src/FunctionHandler.h"
#include <regex>
#include <sstream>
#include <string>
#include <tree/ErrorNode.h>


#define ASSERT_REGEX(line, Regex) \
  std::cerr << line << std::endl;\
  ASSERT_TRUE(\
      std::regex_match(\
        line,\
        Regex\
        )\
      );

TEST(FunctionHandler, CTorFailDuplicateFunctionName){
  std::stringstream mem;
  mem << "path test1 endpath" << std::endl
      << "calculation test2 (var1) returns 5 endcalc" << std::endl
      << "path test2 (var1) endpath" << std::endl
      << "begin end" << std::endl
      << "path test3 (var2) walk 5 endpath" << std::endl
      << "calculation test1 () returns 5 endcalc" << std::endl
      << "calculation test3 (var2) walk 5 returns 5 endcalc" << std::endl
      ;
  getParser(mem);
  auto ast = parser.file();

  ASSERT_TRUE(ast);

  ASSERT_THROW(
      FuctionHandler test(ast->main(), ast->pathdef(), ast->calcdef()),
      const char*
      );
}

TEST(FunctionHandler, CTor){
  std::stringstream mem;
  mem << "path test1 endpath" << std::endl
      << "calculation test2 (var1) returns 5 endcalc" << std::endl
      << "path test3 (var1) endpath" << std::endl
      << "begin end" << std::endl
      << "path test4 (var2) walk 5 endpath" << std::endl
      << "calculation test5 () returns 5 endcalc" << std::endl
      << "calculation test6 (var2) walk 5 returns 5 endcalc" << std::endl
      ;
  getParser(mem);
  auto ast = parser.file();

  ASSERT_TRUE(ast);

  FuctionHandler test(ast->main(), ast->pathdef(), ast->calcdef());

  ASSERT_EQ(test.funcs.size(), 7);
  ASSERT_EQ(test.funcs.at("test1").retType, VarType::VOID);
  ASSERT_STREQ(test.funcs.at("test1").name.c_str(), USR_FUNC_PREFIX "test1" );
  ASSERT_EQ(test.funcs.at("test2").retType, VarType::DOUBLE);
  ASSERT_STREQ(test.funcs.at("test2").name.c_str(), USR_FUNC_PREFIX "test2" );
  ASSERT_EQ(test.funcs.at("test3").retType, VarType::VOID);
  ASSERT_STREQ(test.funcs.at("test3").name.c_str(), USR_FUNC_PREFIX "test3" );
  ASSERT_EQ(test.funcs.at("test4").retType, VarType::VOID);
  ASSERT_STREQ(test.funcs.at("test4").name.c_str(), USR_FUNC_PREFIX "test4" );
  ASSERT_EQ(test.funcs.at("test5").retType, VarType::DOUBLE);
  ASSERT_STREQ(test.funcs.at("test5").name.c_str(), USR_FUNC_PREFIX "test5" );
  ASSERT_EQ(test.funcs.at("test6").retType, VarType::DOUBLE);
  ASSERT_STREQ(test.funcs.at("test6").name.c_str(), USR_FUNC_PREFIX "test6" );
}


TEST(FunctionHandler, getFunctionDeclarations){
  std::stringstream mem;
  mem << "path test1 endpath" << std::endl
      << "calculation test2 (var1) returns 5 endcalc" << std::endl
      << "path test3 (var1) endpath" << std::endl
      << "begin end" << std::endl
      << "path test4 (var2) walk 5 endpath" << std::endl
      << "calculation test5 () returns 5 endcalc" << std::endl
      << "calculation test6 (var2) walk 5 returns 5 endcalc" << std::endl
      ;
  getParser(mem);
  auto ast = parser.file();

  ASSERT_TRUE(ast);

  FuctionHandler test(ast->main(), ast->pathdef(), ast->calcdef());

  std::stringstream testStream;
  test.getFunctionDeclarations(testStream);

  std::regex define("(\\s*\\w+\\s*__usr__func_\\w+\\s*\\((\\s*\\w+\\s+\\w+\\s*|\\s*)\\)\\s*;\\s*$)"
                    "|"
                    "(\\s*void\\s+__env__func_\\w+\\s*\\(\\w+\\s+\\*\\s*\\w+\\s*\\)\\s*;\\s*$)");
  std::string line;
  std::getline(testStream, line);
  ASSERT_REGEX(line, define);

  std::getline(testStream, line);
  ASSERT_REGEX(line, define);

  std::getline(testStream, line);
  ASSERT_REGEX(line, define);

  std::getline(testStream, line);
  ASSERT_REGEX(line, define);

  std::getline(testStream, line);
  ASSERT_REGEX(line, define);

  std::getline(testStream, line);
  ASSERT_REGEX(line, define);

  std::getline(testStream, line);
  ASSERT_REGEX(line, define);
}
