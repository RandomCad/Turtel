#pragma once

#include "src/Function.h"
#include "src/TopLevelVisitor.h"
#include "libs/SceneLexer.h"
#include "libs/SceneParser.h"
#include <sstream>
#include <gtest/gtest.h>
#include <ANTLRInputStream.h>
#include <unordered_map>

using  namespace antlr4;

class TopLevelVisitorTest : public testing::Test {
  protected:
    TopLevelVisitorTest();
    ~TopLevelVisitorTest();
    std::stringstream inputStream;
    std::stringstream retStream;
    TopLevelVisitor toTest;
  private:
    ANTLRInputStream input;
    SceneLexer *lexer;
    CommonTokenStream *tokens;
  protected:
    SceneParser *parser;
    void SetFunction(std::unordered_map<std::string, Function> &);
    void SetFunction(std::unordered_map<std::string, Function> && a) {SetFunction(a);}
    void SetupParser();
};
