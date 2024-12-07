#include "src/VariableHeandler.h"
#include "build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include "src/InternalVarNames.h"
#include "gtest/gtest.h"

TEST(VariableHeandler, TestVariableNameString){
  VariableHeandler handler;
  ASSERT_EQ("__env_posX", handler.getVariableNameString(POS_X));
  ASSERT_EQ("__env_posY", handler.getVariableNameString(POS_Y));
  ASSERT_EQ("__env_rnd", handler.getVariableNameString(RND_NAME));
  ASSERT_EQ("__env_rot", handler.getVariableNameString(ROTATION));
}

TEST(VariableHeandler, TestVariableDefinition){
  VariableHeandler handler;
  ASSERT_EQ("double __env_posX", handler.getVariableDefinition(POS_X));
  ASSERT_EQ("double __env_posY", handler.getVariableDefinition(POS_Y));
  ASSERT_EQ("SDL_Renderer * __env_rnd", handler.getVariableDefinition(RND_NAME));
  ASSERT_EQ("double __env_rot", handler.getVariableDefinition(ROTATION));
}
