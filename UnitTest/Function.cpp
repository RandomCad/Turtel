#include "src/InternalVarNames.h"
#include "src/Function.h"
#include "src/Variable.h"
#include "gtest/gtest.h"
#include <regex>
#include <unordered_map>

#define ASSERT_REGEX(line, Regex) \
  std::cerr << line << std::endl;\
  ASSERT_TRUE(\
      std::regex_match(\
        line,\
        Regex\
        )\
      );

TEST(Function, GetSimplDeclaration){
  Function test("test", {},VarType::VOID, nullptr );
  ASSERT_REGEX(
      test.getFunctionDeclaration(),
      std::regex("\\s*void\\s*\\w+\\s*\\(\\s*\\)\\s*;\\s*")
      );

}
TEST(Function, GetVarDeclaration){
  Function test("TutelMain", 
        {{std::string(RND_NAME),Variable(VarType::RENDERER,"__env_rnd")}}, 
        VarType::VOID,
        nullptr);
  ASSERT_REGEX(
      test.getFunctionDeclaration(),
      std::regex("\\s*void\\s*\\w+\\s*\\(\\s*SDL_Renderer\\s*\\*\\s*__env_rnd\\s*\\)\\s*;\\s*")
      );
}
 
TEST(Function, GetSimplCall){
  Function test("test", {},VarType::VOID, nullptr );
  ASSERT_REGEX(
      test.getFunctionCall({}),
      std::regex("\\s*\\w+\\s*\\(\\s*\\)\\s*;\\s*")
      );

}
TEST(Function, GetVarCall){
  Function test("TutelMain", 
        {{std::string(RND_NAME),Variable(VarType::RENDERER,"__env_rnd")}}, 
        VarType::VOID,
        nullptr);
  ASSERT_REGEX(
      test.getFunctionCall({Variable(VarType::RENDERER,"__usr_rnd")}),
      std::regex("\\s*\\w+\\s*\\(\\s*__usr_rnd\\s*\\)\\s*;\\s*")
      );
}
 
