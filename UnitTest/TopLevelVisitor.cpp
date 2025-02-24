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
#include "src/AstRewriteVisitor.h"
#include "src/VariableVisitor.h"

using  namespace antlr4;

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, AddVar){
  std::stringstream stream;
  stream 
    << "add 5 to test"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);
  
  //pars the test
  auto astStart = parser.addVar();
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
          "\\s*\\_\\_usr\\_\\w+\\s*\\+=\\s*5\\s*;\\s*"
          )
        )
      );

  //TODO add this lines to most of the other tests!
  std::getline(retStream, line);
  std::cerr << line << std::endl;
  ASSERT_TRUE(retStream.eof());
  ASSERT_STREQ(line.c_str(), "");
}

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
  
  auto astStart = parser.walkHome();
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

TEST(TOP_LEVEL_VISITOR_TEST_SUITE, WalkVisit){
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

/*
TEST(TOP_LEVEL_VISITOR_TEST_SUITE, WalkBackVisit){
  std::stringstream stream;
  stream 
    << "walk back 5\n"
    << std::endl;
  ANTLRInputStream input(stream);
  SceneLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);

  auto zwi = parser.stat();
  zwi->accept(new AstRewriteVisitor());
  auto astStart = dynamic_cast<SceneParser::WalkFrontContext*>(zwi->children[0]);

  std::cerr << "test bank1:" << std::endl;
  ASSERT_TRUE(astStart);
  ASSERT_TRUE(astStart->expr());
  ASSERT_EQ(typeid(astStart->expr()), typeid(SceneParser::ExprContext*));
  auto numExpr = dynamic_cast<SceneParser::NegateContext*>( astStart->expr());
  ASSERT_TRUE(numExpr);

  std::cerr << "test bank2 prer:" << std::endl;
  std::stringstream ret;
  VariableHeandler var;
  TopLevelVisitor toTest(ret, var);
  std::any callRet;
  std::cerr << "walking" << std::endl;
  callRet = astStart->accept(&toTest);
    
  std::cerr << "test bank2:" << std::endl;
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
}//*/

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
