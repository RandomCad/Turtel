#include "src/TopLevelVisitor.h"
#include "gtest/gtest.h"
#include <ANTLRInputStream.h>
#include <CommonTokenStream.h>
#include <ParserRuleContext.h>
#include <any>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>

#include "UnitTest/TestHelper.h"
#include "UnitTest/TopLevelVisitor.h"
#include "libs/SceneParser.h"
#include "src/Variable.h"
#include "src/VariableVisitor.h"

std::string getRandomID();
int64_t getRandomInt64();
double getRandomDouble();

#define BoolRetIs(ASSERT) \
  {\
    TopLevelVisitor visitor(zwi, var);\
    auto test = parser.cond();\
    ASSERT_TRUE(test);\
    std::any ret = test->accept(&visitor);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT(std::any_cast<bool>(ret));\
  }
#define Helper(operation1, operation2, type1, type2, rand1, rand2)\
  for(int i = 0; i < TEST_AMMOUNT; ++i){\
    type1 num1 = rand1;\
    type2 num2 = rand2;\
    inputStream\
      << std::setprecision( std::numeric_limits<double>::digits10+2)\
      << num1 << #operation1 << num2\
      ;\
    TopLevelVisitorTest::SetupParser();\
    auto astStart = parser->cond();\
    ASSERT_TRUE(astStart);\
    std::any ret = astStart->accept(&toTest);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT_EQ(std::any_cast<bool>(ret), num1 operation2 num2);\
  }
#define Helper2(type1, type2, rand1, rand2)\
  Helper(>=, >=, type1, type2, rand1, rand2);\
  Helper(>, >, type1, type2, rand1, rand2);\
  Helper(<, <, type1, type2, rand1, rand2);\
  Helper(<=, <=, type1, type2, rand1, rand2);\
  Helper(=, ==, type1, type2, rand1, rand2);\
  Helper(<>, !=, type1, type2, rand1, rand2);

TEST_F(TopLevelVisitorTest, NumSinglCondNum){
  Helper2(int64_t, int64_t, getRandomInt64(), getRandomInt64());
  Helper2(int64_t, double, getRandomInt64(), getRandomDouble());
  Helper2(double, double, getRandomDouble(), getRandomDouble());
  Helper2(double, int64_t, getRandomDouble(), getRandomInt64());
}

#undef Helper
#define Helper(operation1, operation2, type1, type2, rand1, rand2)\
  for(int i = 0; i < TEST_AMMOUNT; ++i){\
    type1 num1 = rand1;\
    type2 num2 = rand2;\
    inputStream\
      << std::setprecision( std::numeric_limits<double>::digits10+2)\
      << '(' << num1 << #operation1 << num2 << ')'\
      ;\
    TopLevelVisitorTest::SetupParser();\
    auto astStart = parser->cond();\
    ASSERT_TRUE(astStart);\
    std::any ret = astStart->accept(&toTest);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT_EQ(std::any_cast<bool>(ret), num1 operation2 num2);\
  }
TEST_F(TopLevelVisitorTest, NumSinglClamdCondNum){
  Helper2(int64_t, int64_t, getRandomInt64(), getRandomInt64());
  Helper2(int64_t, double, getRandomInt64(), getRandomDouble());
  Helper2(double, double, getRandomDouble(), getRandomDouble());
  Helper2(double, int64_t, getRandomDouble(), getRandomInt64());
}

#undef Helper
#define Helper(operation1, operation2, type1, type2, rand1, rand2)\
  for(int i = 0; i < TEST_AMMOUNT; ++i){\
    type1 num1 = rand1;\
    type2 num2 = rand2;\
    inputStream\
      << std::setprecision( std::numeric_limits<double>::digits10+2)\
      << "NOT " << num1 << #operation1 << num2\
      ;\
    TopLevelVisitorTest::SetupParser();\
    auto astStart = parser->cond();\
    ASSERT_TRUE(astStart);\
    std::any ret = astStart->accept(&toTest);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT_EQ(std::any_cast<bool>(ret), !(num1 operation2 num2));\
  }
TEST_F(TopLevelVisitorTest, NotNumSinglCondNum){
  Helper2(int64_t, int64_t, getRandomInt64(), getRandomInt64());
  Helper2(int64_t, double, getRandomInt64(), getRandomDouble());
  Helper2(double, double, getRandomDouble(), getRandomDouble());
  Helper2(double, int64_t, getRandomDouble(), getRandomInt64());
}

#undef Helper
#define Helper(operation1, operation2, type1, type2, rand1, rand2)\
  for(int i = 0; i < TEST_AMMOUNT; ++i){\
    type1 num11 = rand1;\
    type1 num12 = rand1;\
    type2 num21 = rand2;\
    type2 num22 = rand2;\
    inputStream\
      << std::setprecision( std::numeric_limits<double>::digits10+2)\
      << num11 << #operation1 << num21\
      << " AND "\
      << num12 << #operation1 << num22\
      ;\
    TopLevelVisitorTest::SetupParser();\
    auto astStart = parser->cond();\
    ASSERT_TRUE(astStart);\
    std::any ret = astStart->accept(&toTest);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT_EQ(std::any_cast<bool>(ret), (num11 operation2 num21) && (num12 operation2 num22));\
  }
TEST_F(TopLevelVisitorTest, AndNumSinglCondNum){
  Helper2(int64_t, int64_t, getRandomInt64(), getRandomInt64());
  Helper2(int64_t, double, getRandomInt64(), getRandomDouble());
  Helper2(double, double, getRandomDouble(), getRandomDouble());
  Helper2(double, int64_t, getRandomDouble(), getRandomInt64());
}

#undef Helper
#define Helper(operation1, operation2, type1, type2, rand1, rand2)\
  for(int i = 0; i < TEST_AMMOUNT; ++i){\
    type1 num11 = rand1;\
    type1 num12 = rand1;\
    type2 num21 = rand2;\
    type2 num22 = rand2;\
    inputStream\
      << std::setprecision( std::numeric_limits<double>::digits10+2)\
      << num11 << #operation1 << num21\
      << " OR "\
      << num12 << #operation1 << num22\
      ;\
    TopLevelVisitorTest::SetupParser();\
    auto astStart = parser->cond();\
    ASSERT_TRUE(astStart);\
    std::any ret = astStart->accept(&toTest);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT_EQ(std::any_cast<bool>(ret), (num11 operation2 num21) || (num12 operation2 num22));\
  }
TEST_F(TopLevelVisitorTest, OrNumSinglCondNum){
  Helper2(int64_t, int64_t, getRandomInt64(), getRandomInt64());
  Helper2(int64_t, double, getRandomInt64(), getRandomDouble());
  Helper2(double, double, getRandomDouble(), getRandomDouble());
  Helper2(double, int64_t, getRandomDouble(), getRandomInt64());
}

