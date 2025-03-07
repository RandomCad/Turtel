#include <istream>
#include <ANTLRInputStream.h>

#include "libs/SceneParser.h"
#include "libs/SceneLexer.h"

#define getParser(in)\
  antlr4::ANTLRInputStream input(in);\
  SceneLexer lexer(&input);\
  antlr4::CommonTokenStream tokens(&lexer);\
  SceneParser parser(&tokens);

#define TEST_EOF\
  {\
    std::string local_line_local\
    std::getline(retStream, local_line_local);\
    std::cerr << local_line_local << std::endl;\
    ASSERT_TRUE(retStream.eof());\
    ASSERT_STREQ(local_line_local.c_str(), "");\
  }

template<typename retType>
retType parseStream(std::istream &in, retType (SceneParser::*method)()){
  getParser(in);
  return (parser.*method)();
}
