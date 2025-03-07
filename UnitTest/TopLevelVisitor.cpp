#include "src/TopLevelVisitor.h"
#include "gtest/gtest.h"
#include <ANTLRInputStream.h>
#include <any>
#include <cstdint>
#include <gtest/gtest.h>
#include <regex>
#include <string>

#include "libs/SceneParser.h"
#include "libs/SceneLexer.h"
#include "src/VariableHeandler.h"
#include "src/VariableVisitor.h"

std::regex matchKomment = std::regex("\\s*\\/\\/.*$");
std::regex matchPragmaUnrolle = std::regex("\\s*#pragma\\s+unroll\\s*");
std::regex matchClosingCrlBracket = std::regex("\\s*\\}\\s*");

using  namespace antlr4;

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, SingleIf){
  std::stringstream stream;
  stream 
    << "if 5 = 5 then\n"
    << "  store 5 in zwi\n"
    << "endif\n"
    << "if 5 <> 5 then\n"
    << "  store 5 in zwi\n"
    << "endif\n"
    << "if 5 = zwi then\n"
    << "  store 5 in zwi\n"
    << "endif\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  {
    std::cerr << __func__ << " Test1" << std::endl;
    //pars the test
    auto astStart = parser.if_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\/\\/.*$"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test2" << std::endl;
    //pars the test
    auto astStart = parser.if_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\/\\/.*$"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test3" << std::endl;
    //pars the test
    auto astStart = parser.if_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*if\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\{\\s*"
            )
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\}\\s*"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, ElseIf){
  std::stringstream stream;
  stream 
    << "if 5 = 5 then\n"
    << "  store 5 in zwi\n"
    << "else\n"
    << "  store -5 in zwi\n"
    << "endif\n"
    << "if 5 <> 5 then\n"
    << "  store 5 in zwi\n"
    << "else\n"
    << "  store -5 in zwi\n"
    << "endif\n"
    << "if 5 = zwi then\n"
    << "  store 5 in zwi\n"
    << "else\n"
    << "  store -5 in zwi\n"
    << "endif\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  {
    std::cerr << __func__ << " Test1" << std::endl;
    //pars the test
    auto astStart = parser.if_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\/\\/.*$"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test2" << std::endl;
    //pars the test
    auto astStart = parser.if_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\/\\/.*$"
            )
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*-5\\s*;\\s*"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test3" << std::endl;
    //pars the test
    auto astStart = parser.if_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*if\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\{\\s*"
            )
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\}\\s*"
            )
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*else\\s*\\{\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*-5\\s*;\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\}\\s*"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, While){
  std::stringstream stream;
  stream 
    << "while 5 = 5 do\n"
    << "  store 5 in zwi\n"
    << "done\n"
    << "while 5 <> 5 do\n"
    << "  store 5 in zwi\n"
    << "done\n"
    << "while 5 = zwi do\n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  {
    std::cerr << __func__ << " Test1" << std::endl;
    //pars the test
    auto astStart = parser.while_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);
    toTest.infinitLoopFlag = 1;

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchKomment
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*while\\s*\\(\\s*1\\s*\\)\\s*\\{\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\}\\s*"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test2" << std::endl;
    //pars the test
    auto astStart = parser.while_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\/\\/.*$"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test3" << std::endl;
    //pars the test
    auto astStart = parser.while_();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*while\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\{\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\}\\s*"
            )
          )
        );
    
    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, DoUntil){
  std::stringstream stream;
  stream 
    << "repeat\n"
    << "  store 5 in zwi\n"
    << "untile 5 <> 5\n"
    << "repeat\n"
    << "  store 5 in zwi\n"
    << "untile 5 = 5\n"
    << "repeat\n"
    << "  store 5 in zwi\n"
    << "untile 5 = zwi\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  std::regex doStart = std::regex("\\s*do\\s*\\{\\s*");
  
  {
    std::cerr << __func__ << " Test1" << std::endl;
    //pars the test
    auto astStart = parser.doUntil();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);
    toTest.infinitLoopFlag = 1;

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchKomment
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          doStart
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\}\\s*while\\s*\\(\\s*1\\s*\\)\\s*;\\s*"
            )
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test2" << std::endl;
    //pars the test
    auto astStart = parser.doUntil();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchKomment
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test3" << std::endl;
    //pars the test
    auto astStart = parser.doUntil();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          doStart
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
            "\\s*\\}\\s*while\\s*\\(\\s*!\\s*\\(\\s*\\d+\\s*==\\s*__usr\\w+\\s*\\)\\s*\\)\\s*;\\s*"
            )
          )
        );    
    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, ToFor){
  std::stringstream stream;
  stream 
    << "do 5 times \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << "do 5.5 times \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << "do zwi times \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  std::regex forContent = std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*");
  
  {
    std::cerr << __func__ << " Test1" << std::endl;
    //pars the test
    auto astStart = parser.toFor();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchPragmaUnrolle
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
              "\\s*for\\s*\\(\\s*size_t\\s*i\\s*=\\s*0\\s*;\\s*i\\s*<\\s*5\\s*;\\s*\\+\\+i\\s*\\)\\s*\\{\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          forContent
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchClosingCrlBracket
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    std::cerr << __func__ << " Test2" << std::endl;
    //pars the test
    auto astStart = parser.toFor();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchPragmaUnrolle
          )
        );
    
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
              "\\s*for\\s*\\(\\s*size_t\\s*i\\s*=\\s*0\\s*;\\s*i\\s*<\\s*6\\s*;\\s*\\+\\+i\\s*\\)\\s*\\{\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          forContent
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchClosingCrlBracket
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }  
  {
    std::cerr << __func__ << " Test3" << std::endl;
    //pars the test
    auto astStart = parser.toFor();
    ASSERT_TRUE(astStart);

    std::stringstream retStream;
    VariableHeandler var;
    var.setContext(VarVisitor().getVariableContext(astStart));
    TopLevelVisitor toTest(retStream, var);

    std::any ret = astStart->accept(&toTest);
    
    std::string line;
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          std::regex(
              "\\s*for\\s*\\(\\s*size_t\\s*i\\s*=\\s*0\\s*;\\s*i\\s*<\\s*\\w+\\s*;\\s*\\+\\+i\\s*\\)\\s*\\{\\s*"
            )
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          forContent
          )
        );

    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          line,
          matchClosingCrlBracket
          )
        );

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}
///use define for all the tests
#define Base(TestNumber) \
    std::cerr << __func__ << " "#TestNumber << std::endl;\
    auto astStart = parser.for_();\
    ASSERT_TRUE(astStart);\
    std::stringstream retStream;\
    VariableHeandler var;\
    var.setContext(VarVisitor().getVariableContext(astStart));\
    TopLevelVisitor toTest(retStream, var);\
    std::any ret = astStart->accept(&toTest);\
    std::string line;
#define ASSERT_REGEX(Regex) \
  std::getline(retStream, line);\
    std::cerr << line << std::endl;\
    ASSERT_TRUE(\
        std::regex_match(\
          line,\
          Regex\
          )\
        );

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, SimplUpFor){
  std::stringstream stream;
  stream 
    ///test int int
    << "counter i from 0 to 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double int
    << "counter i from 0.1 to 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test int double 
    << "counter i from 0 to 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double double 
    << "counter i from 0.1 to 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test var int 
    //wont test mor at the moment
    << "counter i from zwi to 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  std::regex forContent = std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*");

  {
    Base(Test1);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*<\\s*5\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test2);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*<\\s*5\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test3);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*<\\s*5.5\\d*\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test4);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*<\\s*5.5\\d*\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test5);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;\\s*\\w+\\s*<\\s*5\\s*;\\s*\\+\\+\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, SimplDownFor){
  std::stringstream stream;
  stream 
    ///test int int
    << "counter i from 0 downto 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double int
    << "counter i from 0.1 downto 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test int double 
    << "counter i from 0 downto 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double double 
    << "counter i from 0.1 downto 5.5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test var int 
    //wont test mor at the moment
    << "counter i from zwi downto 5 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  std::regex forContent = std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*");

  {
    Base(Test1);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test2);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test3);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0\\s*;\\s*\\w+\\s*>\\s*5.5\\d*\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test4);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*0.1\\d*\\s*;\\s*\\w+\\s*>\\s*5.5\\d*\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test5);
    ASSERT_REGEX(std::regex("\\s*for\\s*\\(\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;\\s*\\w+\\s*>\\s*5\\s*;\\s*\\-\\-\\w+\\s*\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, StepUpFor){
  std::stringstream stream;
  stream 
    ///test int int
    << "counter i from 0 to 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double int
    << "counter i from 0.1 to 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test int double 
    << "counter i from 0 to 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double double 
    << "counter i from 0.1 to 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test var int 
    //wont test mor at the moment
    << "counter i from zwi to 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  std::regex forContent = std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*");

  {
    Base(Test1);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0\\s*;"
          "\\s*\\w+\\s*<\\s*5\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test2);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0.1\\d*\\s*;"
          "\\s*\\w+\\s*<\\s*5\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test3);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0\\s*;"
          "\\s*\\w+\\s*<\\s*5.5\\d*\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test4);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0.1\\d*\\s*;"
          "\\s*\\w+\\s*<\\s*5.5\\d*\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test5);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;"
          "\\s*\\w+\\s*<\\s*5\\s*;"
          "\\s*\\w+\\s*\\+=\\s*2\\s*"
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, StepDownFor){
  std::stringstream stream;
  stream 
    ///test int int
    << "counter i from 0 downto 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double int
    << "counter i from 0.1 downto 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test int double 
    << "counter i from 0 downto 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test double double 
    << "counter i from 0.1 downto 5.5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    ///test var int 
    //wont test mor at the moment
    << "counter i from zwi downto 5 step 2 do \n"
    << "  store 5 in zwi\n"
    << "done\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  std::regex forContent = std::regex("\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*");

#define forAddition "\\s*\\w+\\s*\\-=\\s*2\\s*"
#define forCondFront "\\s*\\w+\\s*>\\s*"
  {
    Base(Test1);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0\\s*;"
          forCondFront"5\\s*;"
          forAddition
          "\\)\\s*\\{\\s*"));
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test2);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0.1\\d*\\s*;"
          forCondFront"5\\s*;"
          forAddition
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test3);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0\\s*;"
          forCondFront"5.5\\d*\\s*;"
          forAddition
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test4);
    ASSERT_REGEX(matchPragmaUnrolle);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*0.1\\d*\\s*;"
          forCondFront"5.5\\d*\\s*;"
          forAddition
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
  {
    Base(Test5);
    ASSERT_REGEX(
        std::regex(
          "\\s*for\\s*\\("
          "\\s*\\w+\\s*=\\s*__usr_\\w+\\s*;"
          forCondFront"5\\s*;"
          forAddition
          "\\)\\s*\\{\\s*"
          )
        );
    ASSERT_REGEX(forContent);
    ASSERT_REGEX(matchClosingCrlBracket);

    //TODO add this lines to most of the other tests!
    std::getline(retStream, line);
    std::cerr << line << std::endl;
    ASSERT_TRUE(retStream.eof());
    ASSERT_STREQ(line.c_str(), "");
  }
}
#undef forAddition
#undef forCondFront
#undef Base
#undef ASSERT_REGEX

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, MultVar){
  std::stringstream stream;
  stream 
    << "mul test by 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  //pars the test
  auto astStart = parser.multVar();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  var.setContext(VarVisitor().getVariableContext(astStart));
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\_\\_usr\\_\\w+\\s*\\*=\\s*5\\s*;\\s*"
          )
        )
      );

  //TODO add this lines to most of the other tests!
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, DivVar){
  std::stringstream stream;
  stream 
    << "div test by 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  //pars the test
  auto astStart = parser.divVar();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  var.setContext(VarVisitor().getVariableContext(astStart));
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\_\\_usr\\_\\w+\\s*/=\\s*5\\s*;\\s*"
          )
        )
      );

  //TODO add this lines to most of the other tests!
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, SubVar){
  std::stringstream stream;
  stream 
    << "sub 5 from test"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  //pars the test
  auto astStart = parser.subVar();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  var.setContext(VarVisitor().getVariableContext(astStart));
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\_\\_usr\\_\\w+\\s*-=\\s*5\\s*;\\s*"
          )
        )
      );

  //TODO add this lines to most of the other tests!
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, StoreVar){
  std::stringstream stream;
  stream 
    << "store 5 in test"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  //pars the test
  auto astStart = parser.storeVar();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  var.setContext(VarVisitor().getVariableContext(astStart));
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\_\\_usr\\_\\w+\\s*=\\s*5\\s*;\\s*"
          )
        )
      );

  //TODO add this lines to most of the other tests!
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, StopNOK){
  std::stringstream stream;
  stream 
    << "stop 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.stop();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*__envfunc_stop\\s*\\(\\s*5\\s*,\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, StopOK){
  std::stringstream stream;
  stream 
    << "stop"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.stop();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*__envfunc_stop\\s*\\(\\s*0\\s*,\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, FinishNOK){
  std::stringstream stream;
  stream 
    << "finish 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.finish();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*__envfunc_fin\\s*\\(\\s*5\\s*,\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, FinishOK){
  std::stringstream stream;
  stream 
    << "finish"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.finish();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*__envfunc_fin\\s*\\(\\s*0\\s*,\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, Clear){
  std::stringstream stream;
  stream 
    << "clear"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.clear();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*SDL_RenderClear\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, TurnRight){
  std::stringstream stream;
  stream 
    << "turn right 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.turnRight();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*\\+=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, TurnImplizitRight){
  std::stringstream stream;
  stream 
    << "turn 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.turnRight();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*\\+=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, TurnLeft){
  std::stringstream stream;
  stream 
    << "turn left 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.turnLeft();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*\\-=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, Direction){
  std::stringstream stream;
  stream 
    << "direction 5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.direction();
  ASSERT_TRUE(astStart);
  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*\\=\\s*\\(\\s*5\\s*\\)\\s*\\*\\s*\\(\\s*M_PI\\s*\\/\\s*180\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, JumpHome){
  std::stringstream stream;
  stream 
    << "jump home"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.jumpHome();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s+\\/\\s+2;\\s*"
          )
        )
      );

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, WalkHome){
  std::stringstream stream;
  stream 
    << "walk home"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.waklHome();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s+SDL_RenderDrawLine\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s+\\/\\s+2\\s*,\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s+\\/\\s+2;\\s*"
          )
        )
      );

  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, AcceptNumContext){
  std::stringstream stream;
  stream 
    << "5"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  auto astStart = parser.number();
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(dynamic_cast<SceneParser::NumberContext*>(astStart));
  ASSERT_TRUE(dynamic_cast<SceneParser::IntContext*>(astStart));

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);
  ASSERT_EQ(std::any_cast<int64_t>(ret), 5);
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, WalkFrontVisit){
  std::stringstream stream;
  stream 
    << "walk 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = dynamic_cast<SceneParser::WalkFrontContext*>(parser.walk());
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(astStart->expr());
  ASSERT_EQ(typeid(astStart->expr()), typeid(SceneParser::ExprContext*));
  auto numExpr = dynamic_cast<SceneParser::NumExprContext*>( astStart->expr());
  ASSERT_TRUE(numExpr);
  ASSERT_TRUE(numExpr->number());
  ASSERT_EQ(numExpr->number()->getText(), "5");

  std::stringstream ret;
  VariableHeandler var;
  TopLevelVisitor toTest(ret, var);
  std::any callRet;
  callRet = astStart->accept(&toTest);
    
  ASSERT_FALSE(callRet.has_value());

  ret.flush();

  std::string line;
  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s+SDL_RenderDrawLine\\s*\\("
          "\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*,"
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*\\"
          ")\\s*;\\s*"
          )
        )
      );

  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*="
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );

  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*="
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}

// Test für den "mark"-Befehl
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, MarkCommand) {
  std::stringstream stream;
  stream << "mark" << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.mark();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  std::getline(retStream, line);
  std::cerr << "MarkCommand output: " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*pushMarker\\(\\(Marker\\)\\{\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*\\}\\);\\s*")
  ));
}

// Test für den "walk mark"-Befehl
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, WalkMarkCommand) {
  std::stringstream stream;
  stream << "walk mark" << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.walkMark();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  

  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 1): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*if \\(markerStackTop >= 0\\) \\{\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 2): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*Marker m = popMarker\\(\\);\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 3): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*SDL_RenderDrawLine\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*m\\.posX,\\s*m\\.posY\\s*\\)\\s*;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 4): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posX;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 5): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posY;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 6): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.rotation;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "WalkMarkCommand output (Zeile 7): " << line << std::endl;

  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\}\\s*(else\\s*\\{)?\\s*")
  ));
}

// Test für den "jump mark"-Befehl
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, JumpMarkCommand) {
  std::stringstream stream;
  stream << "jump mark" << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.jumpMark();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);
  std::any ret = astStart->accept(&toTest);

  std::string line;
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 1): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*if \\(markerStackTop >= 0\\) \\{\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 2): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*Marker m = popMarker\\(\\);\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 3): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posX;\\s*")
  ));

  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 4): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.posY;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 5): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*m\\.rotation;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "JumpMarkCommand output (Zeile 6): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\}\\s*(else\\s*\\{)?\\s*")
  ));
}

// Test für den "color"-Befehl
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, ColorCommand) {
  std::stringstream stream;
  stream << "color 255 0 128" << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.colorCmd();
  ASSERT_TRUE(astStart);

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);

  std::string line;
  
  std::getline(retStream, line);
  std::cerr << "ColorCommand output (Zeile 1): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*SDL_SetRenderDrawColor\\s*\\(\\s*\\w+\\s*,\\s*255\\s*,\\s*0\\s*,\\s*128\\s*,\\s*255\\s*\\)\\s*;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "ColorCommand output (Zeile 2): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*255\\s*;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "ColorCommand output (Zeile 3): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*0\\s*;\\s*")
  ));
  
  std::getline(retStream, line);
  std::cerr << "ColorCommand output (Zeile 4): " << line << std::endl;
  ASSERT_TRUE(std::regex_match(
    line,
    std::regex("\\s*\\w+\\s*=\\s*128\\s*;\\s*")
  ));
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, WalkBackVisit){
  std::stringstream stream;
  stream 
    << "walk back 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = dynamic_cast<SceneParser::WalkBackContext*>(parser.walk());
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(astStart->expr());
  ASSERT_EQ(typeid(astStart->expr()), typeid(SceneParser::ExprContext*));
  auto numExpr = dynamic_cast<SceneParser::NumExprContext*>( astStart->expr());
  ASSERT_TRUE(numExpr);
  ASSERT_TRUE(numExpr->number());
  ASSERT_EQ(numExpr->number()->getText(), "5");

  std::stringstream ret;
  VariableHeandler var;
  TopLevelVisitor toTest(ret, var);
  std::any callRet;
  callRet = astStart->accept(&toTest);
    
  ASSERT_FALSE(callRet.has_value());

  ret.flush();

  std::string line;
  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s+SDL_RenderDrawLine\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*cos\\(\\s*\\w+\\s*\\)\\s*,\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*sin\\(\\s*\\w+\\s*\\)\\s*\\)\\s*;\\s*"
          )
        )
      );

  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*cos\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );

  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*sin\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, JumpVisit){
  std::stringstream stream;
  stream 
    << "jump 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = dynamic_cast<SceneParser::JumpFrontContext*>(parser.jump());
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(astStart->expr());
  ASSERT_EQ(typeid(astStart->expr()), typeid(SceneParser::ExprContext*));
  auto numExpr = dynamic_cast<SceneParser::NumExprContext*>( astStart->expr());
  ASSERT_TRUE(numExpr);
  ASSERT_TRUE(numExpr->number());
  ASSERT_EQ(numExpr->number()->getText(), "5");

  std::stringstream ret;
  VariableHeandler var;
  TopLevelVisitor toTest(ret, var);
  std::any callRet;
  callRet = astStart->accept(&toTest);
    
  ASSERT_FALSE(callRet.has_value());

  ret.flush();

  std::string line;
  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*="
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*cos\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );

  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*="
          "\\s*\\w+\\s*\\+\\s*\\w+\\s*\\*\\s*sin\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, JumpBackVisit){
  std::stringstream stream;
  stream 
    << "jump back 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = dynamic_cast<SceneParser::JumpBackContext*>(parser.jump());
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(astStart->expr());
  ASSERT_EQ(typeid(astStart->expr()), typeid(SceneParser::ExprContext*));
  auto numExpr = dynamic_cast<SceneParser::NumExprContext*>( astStart->expr());
  ASSERT_TRUE(numExpr);
  ASSERT_TRUE(numExpr->number());
  ASSERT_EQ(numExpr->number()->getText(), "5");

  std::stringstream ret;
  VariableHeandler var;
  TopLevelVisitor toTest(ret, var);
  std::any callRet;
  callRet = astStart->accept(&toTest);
    
  ASSERT_FALSE(callRet.has_value());

  ret.flush();

  std::string line;
  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*cos\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );

  std::getline(ret, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(
      std::regex_match(
        line,
        std::regex(
          "\\s*\\w+\\s*=\\s*\\w+\\s*\\+\\s*-\\s*\\(\\s*\\w+\\s*\\)\\s*\\*\\s*sin\\(\\s*\\w+\\s*\\)\\s*;\\s*"
          )
        )
      );
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, save){
  std::stringstream stream;
  stream 
    << "save test\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto astStart = parser.save();
  ASSERT_TRUE(astStart);

  std::stringstream ret;
  VariableHeandler var;
  TopLevelVisitor toTest(ret, var);
  std::any callRet;
  callRet = astStart->accept(&toTest);
    
  ASSERT_FALSE(callRet.has_value());
}

#define BoolRetIs(ASSERT) \
  {\
    TopLevelVisitor visitor(zwi, var);\
    auto test = parser.cond();\
    ASSERT_TRUE(test);\
    std::any ret = test->accept(&visitor);\
    ASSERT_EQ(ret.type(), typeid(bool));\
    ASSERT(std::any_cast<bool>(ret));\
  }
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, LesEquThan){
  std::stringstream stream;
  stream
    << "5 <= 3\n"
    << "3 <= 5\n"
    << "3 <= 3\n"
    << "var <= 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*<=\\s*3\\s*"
            )
          )
        );
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, LesThan){
  std::stringstream stream;
  stream
    << "5 < 3\n"
    << "3 < 5\n"
    << "3 < 3\n"
    << "var < 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*<\\s*3\\s*"
            )
          )
        );
  }
} 

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, GreaterEquThan){
  std::stringstream stream;
  stream
    << "5 >= 3\n"
    << "3 >= 5\n"
    << "3 >= 3\n"
    << "var >= 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  { 
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_FALSE(std::any_cast<bool>(ret));
  }
  {
    TopLevelVisitor visitor(zwi, var);

    auto test = parser.cond();

    ASSERT_TRUE(test);

    std::any ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(bool));
    ASSERT_TRUE(std::any_cast<bool>(ret));
  }
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>=\\s*3\\s*"
            )
          )
        );
  }
}
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, GreaterThan){
  std::stringstream stream;
  stream
    << "3 > 5\n"
    << "5 > 3\n"
    << "3 > 3\n"
    << "var > 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*3\\s*"
            )
          )
        );
  }
} 

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, CondEqual){
  std::stringstream stream;
  stream
    << "3 = 5\n"
    << "5 = 3\n"
    << "3 = 3\n"
    << "var = 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*==\\s*3\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, CondUnequal){
  std::stringstream stream;
  stream
    << "3 <> 5\n"
    << "5 <> 3\n"
    << "3 <> 3\n"
    << "var <> 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  {
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*!=\\s*3\\s*"
            )
          )
        );
  }
} 

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, ClamCond){
  std::stringstream stream;
  stream
    << "(3 > 5)\n"
    << "(5 > 3)\n"
    << "(3 > 3)\n"
    << "(var > 3)\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\(\\s*\\w+\\s*>\\s*3\\s*\\)\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, NotCond){
  std::stringstream stream;
  stream
    << "NOT 3 > 5\n"
    << "NOT 5 > 3\n"
    << "NOT 3 > 3\n"
    << "NOT var > 3\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_TRUE)
  BoolRetIs(ASSERT_FALSE)
  BoolRetIs(ASSERT_TRUE)
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*!\\(\\s*\\w+\\s*>\\s*3\\s*\\)\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, AndCond){
  std::stringstream stream;
  stream
    << "3 > 5 AND 12 > 12.5\n"
    << "3 > 5 AND 13 > 12.5\n"
    << "5 > 3 AND -3 > 0\n"
    << "5 > 3 AND -3 > -5\n"
    << "var > 3 AND var < 100\n"
    << "var > 3 AND 100 < 100\n"
    << "var > 3 AND 10 < 100\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_TRUE);
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*&&\\s*\\w+\\s*<\\s*\\d+\\s*"
            )
          )
        );
  }
  BoolRetIs(ASSERT_FALSE);
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*"
            )
          )
        );
  }
} 
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, OrCond){
  std::stringstream stream;
  stream
    << "3 > 5 OR 12 > 12.5\n"
    << "3 > 5 OR 13 > 12.5\n"
    << "5 > 3 OR -3 > 0\n"
    << "5 > 3 OR -3 > -5\n"
    << "var > 3 OR var < 100\n"
    << "var > 3 OR 100 < 100\n"
    << "var > 3 OR 10 < 100\n"
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  BoolRetIs(ASSERT_FALSE);
  BoolRetIs(ASSERT_TRUE);
  BoolRetIs(ASSERT_TRUE);
  BoolRetIs(ASSERT_TRUE);
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*\\|\\|\\s*\\w+\\s*<\\s*\\d+\\s*"
            )
          )
        );
  }
  { 
    auto test = parser.cond();

    ASSERT_TRUE(test);

    var.setContext(VarVisitor().getVariableContext(test));
    TopLevelVisitor visitor(zwi, var);
    auto ret = test->accept(&visitor);
    ASSERT_EQ(ret.type(), typeid(std::string));
    std::string testRet = std::any_cast<std::string>(ret);

    std::cerr << testRet << std::endl;
    ASSERT_TRUE(
        std::regex_match(
          testRet,
          std::regex(
            "\\s*\\w+\\s*>\\s*\\d+\\s*"
            )
          )
        );
  }
  BoolRetIs(ASSERT_TRUE);
} 
#undef BoolRetIs

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, NumExpr){
  std::stringstream stream;
  stream << "794651" << std::endl << "91231" << std::endl << "6541.89645" << std::endl
    << "786451.168754" << std::endl << ".8765413" << std::endl 
    << ".876541132" << std::endl;

  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  TopLevelVisitor visitor(zwi, var);

  auto test = parser.number();
  ASSERT_TRUE(test);
  int testNum = std::any_cast<int64_t>(test->accept(&visitor));
  ASSERT_EQ(testNum, 794651);

  test = parser.number();
  ASSERT_TRUE(test);
  testNum = std::any_cast<int64_t>(test->accept(&visitor));
  ASSERT_EQ(testNum, 91231);

  test = parser.number();
  ASSERT_TRUE(test);
  double testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, 6541.89645);

  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, 786451.168754);
  
  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, .8765413);

  test = parser.number();
  ASSERT_TRUE(test);
  testDouble = std::any_cast<double>(test->accept(&visitor));
  ASSERT_EQ(testDouble, .876541132);
}

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, NegExpr){
  std::stringstream stream;
  stream
    << -5
    ;
  //test Setup
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  VariableHeandler var;
  std::stringstream zwi;
  TopLevelVisitor visitor(zwi, var);

  auto test = parser.expr();

  ASSERT_TRUE(test);
  SceneParser::NegateContext *neg;
  ASSERT_TRUE(neg = dynamic_cast<SceneParser::NegateContext*>(test));
  ASSERT_EQ(neg->children.size(), 2);
  ASSERT_TRUE(neg->children[0]);
  ASSERT_STREQ(neg->children[0]->getText().c_str(), "-");
  ASSERT_TRUE(neg->children[1]);
  ASSERT_TRUE(dynamic_cast<SceneParser::NumberContext*>(neg->children[1]));
}

#if FALSE
bool TestWalkParsing(TestError *&ret){
  std::stringstream stream;
  size_t testNumber = 0;

  //add data
  stream << "walk 5" << std::endl;
  stream << "walk -5walk 20 + 5" << std::endl;
  
  //Test preperation
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  //test
  auto test = parser.walk();
  NOT_NULL_ASSERT(test, ret, testNumber);
  NOT_NULL_ASSERT(test->expr(), ret, testNumber);

  test = parser.walk();
  NOT_NULL_ASSERT(test, ret, ++testNumber);
  NOT_NULL_ASSERT(test->expr(), ret, testNumber);
  NOT_NULL_ASSERT(dynamic_cast<SceneParser::NegateContext*>(test->expr()), ret, testNumber)

  test = parser.walk();
  NOT_NULL_ASSERT(test, ret, ++testNumber);
  NOT_NULL_ASSERT(test->expr(), ret, testNumber);
  NOT_NULL_ASSERT(dynamic_cast<SceneParser::AddContext*>(test->expr()), ret, testNumber)

  return false;
}

bool TestVariableParsing(TestError *&ret){
  const size_t TestAmount = 1000;
  std::stringstream stream;
  std::string cor[TestAmount];

  for (size_t i = 0; i < TestAmount; i++) {
    cor[i] = RandomString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", 1);
    cor[i] += RandomString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@", rand() % (TestAmount)); 

    stream << cor[i] << std::endl;
  }
  {  
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  for (size_t i = 0; i < TestAmount; i++) {
    std::cout << "test" << i << std::endl;
    auto test = parser.var();
    
    NOT_NULL_ASSERT(test, ret, i);
    SceneParser::VariableContext *testVar = dynamic_cast<SceneParser::VariableContext*>(test); 
    std::cout << testVar->ID()->getSymbol()->getText() << std::endl << cor[i] << std::endl;
    NOT_NULL_ASSERT(testVar, ret, i)
    NULL_ASSERT(testVar->exception, ret, i);
    NOT_NULL_ASSERT(testVar->ID(), ret, i);
    NOT_NULL_ASSERT(testVar->ID()->getSymbol(), ret, i)
    STRING_ASSERT(testVar->ID()->getSymbol()->getText(), cor[i], ret, i)
  }
  }

  stream.clear();

  //explisitly test the case of only @
  cor[0] = "@";
  stream << cor[0] << std::endl;

  for (size_t i = 1; i < TestAmount; i++) {
    cor[i] = "@";
    cor[i] += RandomString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@", rand() % (TestAmount)); 

    stream << cor[i] << std::endl;
  }

  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    
  
  for (size_t i = 0; i < TestAmount; i++) {
    std::cout << "test" << i << std::endl;
    auto test = parser.var();
    
    NOT_NULL_ASSERT(test, ret, i);
    SceneParser::GlobalVariableContext *testVar = dynamic_cast<SceneParser::GlobalVariableContext*>(test); 
    NOT_NULL_ASSERT(testVar, ret, i)
    NULL_ASSERT(testVar->exception, ret, i);
    NOT_NULL_ASSERT(testVar->IncID(), ret, i);
    NOT_NULL_ASSERT(testVar->IncID()->getSymbol(), ret, i)
    STRING_ASSERT(testVar->IncID()->getSymbol()->getText(), cor[i], ret, i)
  }


  
  return false;
}

#endif
