///@file  This is the test file for the main function.
///       Therfor Integration tests are performed at this stage

#include "gtest/gtest.h"
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <filesystem>

///handwriten include for the antlr.cpp file.
///The file isn't realy included
int progMain(int agrc, const char *argv[]);
bool CheckSurfaceForBlack(SDL_Surface *a);

TEST(Main, CircleTG2){
  char *argv[5];
  argv[0] = "test";
  argv[1] = "./TestData/circle.tg2";
  argv[2] = "--output";
  argv[3] = "./circle.out";
  argv[4] = 0;

  progMain(2, (const char**)argv);

  int exitCode = std::system((std::string("./") + argv[3]).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);
  {
    std::string pngTest1 = std::string("./circle.png");
    ASSERT_TRUE(std::filesystem::exists(pngTest1));

    SDL_Surface *a = IMG_Load(pngTest1.c_str());

    ASSERT_EQ(a->format->BytesPerPixel, 4);
    ASSERT_FALSE(CheckSurfaceForBlack(a));
  }
}
TEST(Main, CircleTG){
  char *argv[5];
  argv[0] = "test";
  argv[1] = "./TestData/circle.tg";
  argv[2] = "--output";
  argv[3] = "./circle.out";
  argv[4] = 0;

  progMain(2, (const char**)argv);

  int exitCode = std::system((std::string("./") + argv[3]).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);
}

TEST(Main, SpiralTG2){
  char *argv[5];
  argv[0] = "test";
  argv[1] = "./TestData/spiral.tg2";
  argv[2] = "--output";
  argv[3] = "./spiral.out";
  argv[4] = 0;

  progMain(2, (const char**)argv);

  int exitCode = std::system((std::string("./") + argv[3]).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);
  {
    std::string pngTest1 = std::string("./spiral.png");
    ASSERT_TRUE(std::filesystem::exists(pngTest1));

    SDL_Surface *a = IMG_Load(pngTest1.c_str());

    ASSERT_EQ(a->format->BytesPerPixel, 4);
    ASSERT_FALSE(CheckSurfaceForBlack(a));
  }
}
TEST(Main, SpiralTG){
  char *argv[5];
  argv[0] = "test";
  argv[1] = "./TestData/spiral.tg";
  argv[2] = "--output";
  argv[3] = "./spiral.out";
  argv[4] = 0;

  progMain(2, (const char**)argv);

  int exitCode = std::system((std::string("./") + argv[3]).c_str());
  ASSERT_EQ(WEXITSTATUS(exitCode), 0);
}
