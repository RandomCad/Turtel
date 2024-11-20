#include "src/VariableHeandler.h"
#include "build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include "gtest/gtest.h"

TEST(VariableHeandler, TestVariableNameString){
  VariableHeandler handler;
  ASSERT_EQ("__env_posX", handler.getVariableNameString("@x"));
  ASSERT_EQ("__env_posY", handler.getVariableNameString("@y"));
  ASSERT_EQ("__env_rnd", handler.getVariableNameString("rnd"));
}

TEST(VariableHeandler, TestVariableDefinition){
  VariableHeandler handler;
  ASSERT_EQ("double __env_posX", handler.getVariableDefinition("@x"));
  ASSERT_EQ("double __env_posY", handler.getVariableDefinition("@y"));
  ASSERT_EQ("SDL_Renderer * __env_rnd", handler.getVariableDefinition("rnd"));
}
