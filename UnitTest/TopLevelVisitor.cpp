#include "src/TopLevelVisitor.h"
#include "gtest/gtest.h"
#include <ANTLRInputStream.h>
#include <any>
#include <gtest/gtest.h>
#include <regex>
#include <string>

#include "../libs/SceneParser.h"
#include "../libs/SceneLexer.h"
#include "src/VariableHeandler.h"
#include "src/AstRewriteVisitor.h"

using  namespace antlr4;

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

  std::stringstream retStream;
  VariableHeandler var;
  TopLevelVisitor toTest(retStream, var);

  std::any ret = astStart->accept(&toTest);
  ASSERT_FALSE(ret.has_value());
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
    std::regex("\\s*\\}\\s*")
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
    std::regex("\\s*\\}\\s*")
  ));
}

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
