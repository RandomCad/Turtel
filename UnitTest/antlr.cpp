///@file  This is the test file for the main function.
///       Therfor Integration tests are performed at this stage

#include "gtest/gtest.h"

///handwriten include for the antlr.cpp file.
///The file isn't realy included
int progMain(int agrc, const char *argv[]);

TEST(Main, Circle){
  char *argv[5];
  argv[0] = "test";
  argv[1] = "./TestData/circle.tg";
  argv[2] = "--output";
  argv[3] = "./circle.out";
  argv[4] = 0;

  progMain(2, (const char**)argv);

  int exitCode = std::system((std::string("./") + argv[3]).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 36);
}
