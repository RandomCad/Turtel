///@file  This is the test file for the main function.
///       Therfor Integration tests are performed at this stage

#include "build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include "gtest/gtest.h"

///handwriten include for the antlr.cpp file.
///The file isn't realy included
int progMain(int agrc, const char *argv[]);

TEST(Main, Circle){
  char *argv[3];
  argv[0] = "test";
  argv[1] = "./TestData/circle.tg";
  argv[2] = 0;

  progMain(2, (const char**)argv);

}
