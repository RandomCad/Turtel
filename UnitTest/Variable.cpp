#include "src/Variable.h"
#include "gtest/gtest.h"

TEST(Variable, TestGetName){
  Variable toTest(VarType::DOUBLE, "test");

  EXPECT_FALSE(toTest.isUnique);
  EXPECT_EQ(toTest.type, VarType::DOUBLE);
  EXPECT_EQ(toTest.name, "test");

  ASSERT_EQ(toTest.getName(), "test");
}

TEST(Variable, TestGetNameAndType){
  Variable toTest(VarType::DOUBLE, "test");

  EXPECT_FALSE(toTest.isUnique);
  EXPECT_EQ(toTest.type, VarType::DOUBLE);
  EXPECT_EQ(toTest.name, "test");
  EXPECT_EQ(toTest.getName(), "test");

  ASSERT_EQ(toTest.getTypeAndName(), "double test");
}
