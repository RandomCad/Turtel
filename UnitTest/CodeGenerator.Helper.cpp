#include "src/CodeGenerator.Helper.h"
#include "src/VariableHeandler.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>
#include <regex>

TEST(Code_Generator_Helper, GenPresent){
  VariableHeandler var;
  std::stringstream out;

  GenPresent(var,out);

  std::string test;

  std::getline(out, test);
  std::cerr << test << std::endl;
  ASSERT_TRUE(std::regex_match(test, std::regex(
    "\\s+SDL_SetRenderTarget\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*\\)\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));
 
  std::getline(out, test);
  std::cerr << test << std::endl;
  ASSERT_TRUE(std::regex_match(test, std::regex(
    "\\s+SDL_RenderClear\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*",
    std::regex_constants::ECMAScript
  ))); 

  std::getline(out, test);
  std::cerr << test << std::endl;
  ASSERT_TRUE(std::regex_match(test, std::regex(
    "\\s+SDL_RenderCopy\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*,\\s*\\w+\\s*\\)\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));

  std::getline(out, test);
  std::cerr << test << std::endl;
  ASSERT_TRUE(std::regex_match(test, std::regex(
    "\\s+SDL_RenderPresent\\s*\\(\\s*\\w+\\s*\\)\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));

  std::getline(out, test);
  std::cerr << test << std::endl;
  ASSERT_TRUE(std::regex_match(test, std::regex(
    "\\s+SDL_SetRenderTarget\\s*\\(\\s*\\w+\\s*,\\s*\\w+\\s*\\)\\s*;\\s*",
    std::regex_constants::ECMAScript
  )));
}

TEST(Code_Generator_Helper, CalcPosX){
  std::stringstream res;
  ASSERT_EQ(res.str().length(), 0);

  { 
    CalcPosX<int>(5, res, "test", "test");
    ASSERT_EQ(res.str(), "test + 5 * cos(test)");
    res.str("");

    CalcPosX<int>(5, res, "test", "Test");
    ASSERT_EQ(res.str(), "test + 5 * cos(Test)");
    res.str("");

    CalcPosX<long>(5, res, "d", "Test");
    ASSERT_EQ(res.str(), "d + 5 * cos(Test)");
    res.str("");

    CalcPosX<char>(5, res, "d", "get");
    ASSERT_EQ(res.str(), "d + \005 * cos(get)");
    res.str("");

    CalcPosX<char *>((char *)"test", res, "d", "get");
    ASSERT_EQ(res.str(), "d + test * cos(get)");
    res.str("");

    CalcPosX<const char *>("tter", res, "get2", "get");
    ASSERT_EQ(res.str(), "get2 + tter * cos(get)");
    res.str("");

    CalcPosX<std::string>("td::string", res, "get2", "\004");
    ASSERT_EQ(res.str(), "get2 + td::string * cos(\004)");
    res.str("");
  }

  { 
    VariableHeandler vars;
    CalcPosX<int>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posX + 5 * cos(__env_rot)");
    res.str("");

    CalcPosX<int>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posX + 5 * cos(__env_rot)");
    res.str("");

    CalcPosX<long>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posX + 5 * cos(__env_rot)");
    res.str("");

    CalcPosX<char>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posX + \005 * cos(__env_rot)");
    res.str("");

    CalcPosX<char *>((char *)"test", res, vars);
    ASSERT_EQ(res.str(), "__env_posX + test * cos(__env_rot)");
    res.str("");

    CalcPosX<const char *>("tter", res, vars);
    ASSERT_EQ(res.str(), "__env_posX + tter * cos(__env_rot)");
    res.str("");

    CalcPosX<std::string>("std::string", res, vars);
    ASSERT_EQ(res.str(), "__env_posX + std::string * cos(__env_rot)");
    res.str("");
  }
}

TEST(Code_Generator_Helper, CalcPosY){
  std::stringstream res;
  ASSERT_EQ(res.str().length(), 0);

  { 
    CalcPosY<int>(5, res, "test", "test");
    ASSERT_EQ(res.str(), "test + 5 * sin(test)");
    res.str("");

    CalcPosY<int>(5, res, "test", "Test");
    ASSERT_EQ(res.str(), "test + 5 * sin(Test)");
    res.str("");

    CalcPosY<long>(5, res, "d", "Test");
    ASSERT_EQ(res.str(), "d + 5 * sin(Test)");
    res.str("");

    CalcPosY<char>(5, res, "d", "get");
    ASSERT_EQ(res.str(), "d + \005 * sin(get)");
    res.str("");

    CalcPosY<char *>((char *)"test", res, "d", "get");
    ASSERT_EQ(res.str(), "d + test * sin(get)");
    res.str("");

    CalcPosY<const char *>("tter", res, "get2", "get");
    ASSERT_EQ(res.str(), "get2 + tter * sin(get)");
    res.str("");

    CalcPosY<std::string>("td::string", res, "get2", "\004");
    ASSERT_EQ(res.str(), "get2 + td::string * sin(\004)");
    res.str("");
  }

  { 
    VariableHeandler vars;
    CalcPosY<int>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posY + 5 * sin(__env_rot)");
    res.str("");

    CalcPosY<int>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posY + 5 * sin(__env_rot)");
    res.str("");

    CalcPosY<long>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posY + 5 * sin(__env_rot)");
    res.str("");

    CalcPosY<char>(5, res, vars);
    ASSERT_EQ(res.str(), "__env_posY + \005 * sin(__env_rot)");
    res.str("");

    CalcPosY<char *>((char *)"test", res, vars);
    ASSERT_EQ(res.str(), "__env_posY + test * sin(__env_rot)");
    res.str("");

    CalcPosY<const char *>("tter", res, vars);
    ASSERT_EQ(res.str(), "__env_posY + tter * sin(__env_rot)");
    res.str("");

    CalcPosY<std::string>("std::string", res, vars);
    ASSERT_EQ(res.str(), "__env_posY + std::string * sin(__env_rot)");
    res.str("");
  }
}

