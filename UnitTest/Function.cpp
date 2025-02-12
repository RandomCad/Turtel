#include "src/InternalVarNames.h"
#include "src/Function.h"
#include "gtest/gtest.h"
#include <regex>

TEST(HelperFuncs, ImmageFunction){
  HelperFunc toTest(
        IMMAGE_FUNC_NAME,
        std::vector<Variable>{
          Variable(VarType::DOUBLE, "retCode"),
          Variable(VarType::RENDERER, "__env_rnd"),
        },
        VarType::VOID
      );

  std::string decl = toTest.getFunctionDectaration();
  std::cerr << decl << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        decl,
        std::regex(
          "\\s*void\\s+" IMMAGE_FUNC_NAME "\\s*\\"
          "(\\s*double\\s+retCode\\s*"
          ",\\s*SDL_Renderer\\s*\\*\\s*__env_rnd\\s*\\)\\s*;\\s*"
          )
        )
      );

  std::cerr << "diff--------------------" << std::endl;

  std::string call = toTest.getFunctionCall(
      std::vector<Variable>{
        Variable(VarType::DOUBLE, "test1"), 
        Variable(VarType::RENDERER, "test2") 
      });
  std::cerr << call << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        call,
        std::regex(
          "\\s*" IMMAGE_FUNC_NAME "\\s*\\(\\s*test1\\s*,\\s*test2\\s*\\)\\s*"
          )
        )
      );
}
