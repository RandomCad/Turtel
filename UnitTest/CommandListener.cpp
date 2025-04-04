#include "gtest/gtest.h"
#include "libs/CommandlineLexer.h"
#include "libs/CommandlineParser.h"
#include "src/CommandLineListener.h"
#include <sstream>
#include <ANTLRInputStream.h>

TEST(CommandListener, minimal){
  std::stringstream test;
  test << "test test";
  
  antlr4::ANTLRInputStream argInput(test);
  CommandlineLexer argLex(&argInput);
  antlr4::CommonTokenStream argTokens(&argLex);
  CommandlineParser argPars(&argTokens);

  CommandLineListener argLis;
  argPars.addParseListener(&argLis);

  argPars.args();

  Comandline::options toTest = argLis.GetOptions();

  ASSERT_EQ(toTest.inputFile, std::string("test"));
  ASSERT_EQ(toTest.outputFile, std::string("out"));
}
TEST(CommandListener, OutFile){
  std::stringstream test;
  test << "test test --output test";
  
  antlr4::ANTLRInputStream argInput(test);
  CommandlineLexer argLex(&argInput);
  antlr4::CommonTokenStream argTokens(&argLex);
  CommandlineParser argPars(&argTokens);

  CommandLineListener argLis;
  argPars.addParseListener(&argLis);

  argPars.args();

  Comandline::options toTest = argLis.GetOptions();

  ASSERT_EQ(toTest.inputFile, std::string("test"));
  ASSERT_EQ(toTest.outputFile, std::string("test"));
}
