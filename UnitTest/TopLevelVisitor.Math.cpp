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
#include <regex>
#include <sstream>
#include <string>
#include <cmath>

#include "UnitTest/TestHelper.h"
#include "UnitTest/TopLevelVisitor.h"
#include "libs/SceneParser.h"
#include "src/Variable.h"

std::string getRandomID();
int64_t getRandomInt64();
double getRandomDouble();

TEST_F(TopLevelVisitorTest, ExprVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID = getRandomID();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << ID
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(Variable));
  }
}
TEST_F(TopLevelVisitorTest, ExprNumberInt){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t num = getRandomInt64();
    std::cerr << "Test num: " << num << std::endl;
    inputStream
      <<num 
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), num);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumberDouble){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double num = getRandomDouble();
    std::cerr << "Test num: " << std::to_string(num) << std::endl;
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << num 
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), num);
  }
}

TEST_F(TopLevelVisitorTest, ClamVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID = getRandomID();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->klamKon();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(Variable));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID = getRandomID();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(Variable));
  }
}
TEST_F(TopLevelVisitorTest, ClamNumInt){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID = getRandomInt64();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->klamKon();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID = getRandomInt64();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID);
  }
}
TEST_F(TopLevelVisitorTest, ClamNumDouble){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID = getRandomDouble();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->klamKon();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID = getRandomDouble();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID);
  }
}

TEST_F(TopLevelVisitorTest, ExprNegateNumberDouble){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double num = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << '-' << num 
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), -num);
  }
}
TEST_F(TopLevelVisitorTest, ExprNegateNumberInt){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t num = getRandomInt64();
    inputStream
      << '-' << num 
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), -num);
  }
}
TEST_F(TopLevelVisitorTest, ExprNegateVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID = getRandomID();
    inputStream
      << '-' << ID
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "-__usr_" + ID);
  }
}
TEST_F(TopLevelVisitorTest, ExprNegateClamVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID = getRandomID();
    inputStream
      << '-' << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "-__usr_" + ID);
  }
}
TEST_F(TopLevelVisitorTest, ExprNegateClamNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << '-' << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), -ID);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID = getRandomInt64();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << '-' << '(' << ID << ')'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), -ID);
  }
}
TEST_F(TopLevelVisitorTest, ExprNegateClamNegate){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID = getRandomInt64();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << "-(-" <<  ID << ")"
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), -(-ID));
  }
}

TEST_F(TopLevelVisitorTest, ExprAbsVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID = getRandomID();
    std::cerr << "Test ID: " << ID << std::endl;
    inputStream
      << '|' << ID << '|'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_REGEX(std::any_cast<std::string>(ret), std::regex("fabs\\(\\w+\\)"));
  }
}
TEST_F(TopLevelVisitorTest, ExprAbsNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << '|' << ID << '|'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::abs(ID));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID = getRandomInt64();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << '|' << ID << '|'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), std::abs(ID));
  }
}
TEST_F(TopLevelVisitorTest, ExprAbsNegNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << "|-" << ID << '|'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::abs(ID));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID = getRandomInt64();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << "|-" << ID << '|'
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), std::abs(ID));
  }
}

TEST_F(TopLevelVisitorTest, ExprVarAddVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    std::string ID2 = getRandomID();
    inputStream
      << ID1 << '+' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '+' + "__usr_" + ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprVarAddNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '+' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '+' + std::to_string(ID2));
  }
}
TEST_F(TopLevelVisitorTest, ExprNumAddVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID2 << '+' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), std::to_string(ID2) + '+' + "__usr_" + ID1);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumAddNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '+' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 + ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '+' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 + ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << '+' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 + ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '+' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 + ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumAddNegNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << "+-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 + -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "+-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 + -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << "+-" << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), -ID1 + ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "+-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 + -ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMultiAdd){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::vector<int64_t> am;
    for(int i = 0; i < 2 + rand() % TEST_AMMOUNT; ++i) am.push_back(getRandomInt64()); 
    int64_t result = 0;
    std::cerr << am.size();
    for (size_t i = 0; i < am.size() - 1; ++i) {
      inputStream << am[i] << '+';
      result += am[i];
    }
    inputStream << am[am.size() - 1];
    result += am[am.size() - 1];

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), result);
  }
}

TEST_F(TopLevelVisitorTest, ExprVarDimVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    std::string ID2 = getRandomID();
    inputStream
      << ID1 << '-' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '-' + "__usr_" + ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprVarDimNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '-' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '-' + std::to_string(ID2));
  }
}
TEST_F(TopLevelVisitorTest, ExprNumDimVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID2 << '-' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), std::to_string(ID2) + '-' + "__usr_" + ID1);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumDimNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '-' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 - ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '-' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 - ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << '-' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID2 - ID1);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '-' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 - ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumDimNegNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << "--" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 - -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "--" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 - -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << "--" << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID2 - -ID1);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "- -" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 - -ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMultiDim){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::vector<int64_t> am;
    for(int i = 0; i < 2 + rand() % TEST_AMMOUNT; ++i) am.push_back(getRandomInt64()); 
    int64_t result = am[0];
    inputStream << am[0] << '-';
    for (size_t i = 1; i < am.size() - 1; ++i) {
      inputStream << am[i] << '-';
      result -= am[i];
    }
    inputStream << am[am.size() - 1];
    result -= am[am.size() - 1];

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), result);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMultiDimAdd){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::vector<int64_t> am;
    for(int i = 0; i < 2 + rand() % TEST_AMMOUNT; ++i) am.push_back(getRandomInt64()); 
    int64_t result = am[0];
    inputStream << am[0] ;
    for (size_t i = 1; i < am.size() - 1; ++i) {
      if(rand() %2){
        inputStream << '-' << am[i];
        result -= am[i];
      }
      else{
        inputStream << '+' << am[i];
        result += am[i];
      }
    }
    inputStream << '-' << am[am.size() - 1];
    result -= am[am.size() - 1];

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), result);
  }
}

TEST_F(TopLevelVisitorTest, ExprVarDivVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    std::string ID2 = getRandomID();
    inputStream
      << ID1 << '/' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '/' + "__usr_" + ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprVarDivNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '/' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '/' + std::to_string(ID2));
  }
}
TEST_F(TopLevelVisitorTest, ExprNumDivVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID2 << '/' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), std::to_string(ID2) + '/' + "__usr_" + ID1);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumDivNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '/' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 / ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '/' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 / ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << '/' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID2 / ID1);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '/' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 / ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumDivNegNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << "/-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 / -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "/-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 / -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << "/-" << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID2 / -ID1);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "/-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 / -ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMultiDiv){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::vector<int64_t> am;
    for(int i = 0; i < 2 + rand() % TEST_AMMOUNT; ++i) am.push_back(getRandomInt64()); 
    int64_t result = am[0];
    inputStream << am[0] << '/';
    for (size_t i = 1; i < am.size() - 1; ++i) {
      inputStream << am[i] << '/';
      result /= am[i];
    }
    inputStream << am[am.size() - 1];
    result /= am[am.size() - 1];

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), result);
  }
}

TEST_F(TopLevelVisitorTest, ExprVarMulVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    std::string ID2 = getRandomID();
    inputStream
      << ID1 << '*' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '*' + "__usr_" + ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprVarMulNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '*' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "__usr_" + ID1 + '*' + std::to_string(ID2));
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMulVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID2 << '*' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), std::to_string(ID2) + '*' + "__usr_" + ID1);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMulNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '*' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 * ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '*' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 * ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << '*' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID2 * ID1);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '*' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 * ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMulNegNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << "*-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), ID1 * -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "*-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 * -ID2);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << "*-" << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID2 * -ID1);
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "*-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), ID1 * -ID2);
  }
}
TEST_F(TopLevelVisitorTest, ExprNumMultiMul){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::vector<int64_t> am;
    for(int i = 0; i < 2 + rand() % TEST_AMMOUNT; ++i) am.push_back(getRandomInt64()); 
    int64_t result = am[0];
    inputStream << am[0] << '*';
    for (size_t i = 1; i < am.size() - 1; ++i) {
      inputStream << am[i] << '*';
      result *= am[i];
    }
    inputStream << am[am.size() - 1];
    result *= am[am.size() - 1];

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(int64_t));
    ASSERT_EQ(std::any_cast<int64_t>(ret), result);
  }
}

TEST_F(TopLevelVisitorTest, ExprVarExpVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    std::string ID2 = getRandomID();
    inputStream
      << ID1 << '^' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "pow(__usr_" + ID1 + ',' + "__usr_" + ID2 + ')');
  }
}
TEST_F(TopLevelVisitorTest, ExprVarExpNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '^' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "pow(__usr_" + ID1 + ',' + std::to_string(ID2) + ')');
  }
}
TEST_F(TopLevelVisitorTest, ExprNumExpVar){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string ID1 = getRandomID();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID2 << '^' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_EQ(std::any_cast<std::string>(ret), "pow(" + std::to_string(ID2) + ',' + "__usr_" + ID1 + ')');
  }
}
TEST_F(TopLevelVisitorTest, ExprNumExpNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << '^' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_EQ(std::any_cast<double>(ret), std::pow(ID1 , ID2));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '^' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::pow(ID1 , ID2));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << '^' << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::pow(ID2 , ID1));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << '^' << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::pow(ID1 , ID2));
  }
}
TEST_F(TopLevelVisitorTest, ExprNumExpNegNum){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    int64_t ID2 = getRandomInt64();
    inputStream
      << ID1 << "^-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_EQ(std::any_cast<double>(ret), std::pow(ID1 , -ID2));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "^-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::pow(ID1 , -ID2));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t ID1 = getRandomInt64();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID2 << "^-" << ID1
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::pow(ID2 ,-ID1));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    double ID1 = getRandomDouble();
    double ID2 = getRandomDouble();
    inputStream
      << std::setprecision( std::numeric_limits<double>::digits10+2)
      << ID1 << "^-" << ID2
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_DOUBLE_EQ(std::any_cast<double>(ret), std::pow(ID1,-ID2));
  }
}

TEST_F(TopLevelVisitorTest, ExprFunc){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string funcID = getRandomID();
    inputStream
      << funcID << "()"
      ;

    std::stringstream stream;
    stream << "calculation " << funcID << "() returns 0 endcalc" << std::endl;

    antlr4::ANTLRInputStream input1(stream);
    SceneLexer lexer1(&input1);
    antlr4::CommonTokenStream tokens1(&lexer1);
    SceneParser parser1(&tokens1);    

    TopLevelVisitorTest::SetupParser();
    TopLevelVisitorTest::SetFunction({{funcID, Function(funcID, VarType::DOUBLE, parser1.calcdef())}});

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_REGEX(std::any_cast<std::string>(ret), std::regex("\\w+\\(\\)\\s*"));
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    std::string funcID = getRandomID();
    std::string varID = getRandomID();
    inputStream
      << funcID << '(' << varID << ')'
      ;

    std::stringstream stream;
    stream << "calculation " << funcID << '(' << varID << ") returns 0 endcalc" << std::endl;

    antlr4::ANTLRInputStream input1(stream);
    SceneLexer lexer1(&input1);
    antlr4::CommonTokenStream tokens1(&lexer1);
    SceneParser parser1(&tokens1);    

    TopLevelVisitorTest::SetupParser();
    TopLevelVisitorTest::SetFunction({{funcID, Function(funcID, VarType::DOUBLE, parser1.calcdef())}});

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(std::string));
    ASSERT_REGEX(std::any_cast<std::string>(ret), std::regex("\\w+\\(\\s*\\w+\\s*\\)\\s*"));
  }
}

TEST_F(TopLevelVisitorTest, ExprNumComplex){
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t num[16];
    for(int i = 0; i<16;++i) num[i] = getRandomInt64();
    inputStream
      << '(' << num[0] << "+" << num[1] << ")^" << num[3]  << '*' << num[4]
      << "-(" << num[5] << "/(" << num[6] << '+' << num[7] << ")*(" << num[8] << '*' << num[9] << ")/" << num[10] << ')'
      << "+" << num[11] << "^((" << num[12] << '-' << num[13] << ")/(" << num[14] << '*' << num[15] << "))"
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_EQ(std::any_cast<double>(ret),
        std::pow( num[0] + num[1], num[3]) * num[4]
        -( num[5]/(num[6]+num[7]) * (num[8]*num[9])/num[10])
        +std::pow(num[11], (num[12] - num[13])/(num[14]*num[15])) 
        );
          
  }
  for(int i = 0; i < TEST_AMMOUNT; ++i){
    int64_t num[17];
    for(int i = 0; i<17;++i) num[i] = getRandomInt64();
    inputStream
      << '(' << num[0] << "+" << num[1] << ")^" << num[3]  << '*' << num[4]
      << "-(" << num[5] << "/(" << num[6] << '+' << num[7] << ")*(" << num[8] << '*' << num[9] << ")/" << num[10] << ')'
      << "+" << num[11] << "^" << num[16] << "+((" << num[12] << '-' << num[13] << ")/(" << num[14] << '*' << num[15] << "))"
      ;

    TopLevelVisitorTest::SetupParser();

    auto astStart = parser->expr();
    ASSERT_TRUE(astStart);

    TopLevelVisitorTest::SetVariables(astStart);
    std::any ret = astStart->accept(&toTest);

    ASSERT_EQ(ret.type(), typeid(double));
    ASSERT_EQ(std::any_cast<double>(ret),
        std::pow( num[0] + num[1], num[3]) * num[4]
        -( num[5]/(num[6]+num[7]) * (num[8]*num[9])/num[10])
        +std::pow(num[11], num[16]) + (num[12] - num[13])/(num[14]*num[15]) 
        );
          
  }
}

std::string getRandomID(){
  int size = rand() % 1000 + 1;
  std::string ret;
  for (int i = 0;i < size; ++i) {
    int num = rand();
    char next = (num % 26) + 1;
    if(num & 0x80) next |= 0b01000000;
    else           next |= 0b01100000;

    ret += next;
  }
  return ret;
}
int64_t getRandomInt64(){
  return (((int64_t)rand()) << 32) | rand();
}
double getRandomDouble(){
  return ((double)rand()/RAND_MAX) * rand();
}


