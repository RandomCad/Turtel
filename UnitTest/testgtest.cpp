#include <filesystem>
#include <iostream>

#include "../src/LLVMInterface.h"
#include "gtest/gtest.h"
TEST(LLVM_Intervace_test, CallLLVM)
{
  const char *testFile = "TestCallLLVM.out";
  LLVMInterface interface(testFile);

  interface.llvmFile << "#include <stdio.h>\nint main(int argc, const char *argv[]){printf(\"Hello World\\n\");}" << std::endl;

  interface.CallLLVM();
  EXPECT_TRUE( std::filesystem::exists(interface.fileName));

}

TEST(LLVM_Intervace_test, EmptySDL2App){
  const char * testFile = "TestEmptySdl2App.out";
  LLVMInterface interface(testFile);

  interface.llvmFile 
    << "#include <SDL2/SDL.h>" << std::endl 
    << "int main(int arg, const char *argv[]){" << std::endl
    << "  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);" << std::endl
    << "  SDL_Window* window = SDL_CreateWindow( \"Health bar\", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN );" << std::endl
    << "  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);" << std::endl
    << "  SDL_DestroyRenderer(renderer);" << std::endl
    << "  SDL_DestroyWindow(window);" << std::endl
    << "  SDL_Quit();" << std::endl
    << "}" << std::endl
    ;

  interface.CallLLVM();

  EXPECT_TRUE(std::filesystem::exists(interface.fileName));

  interface.llvmFile.close();

  std::cout << "test" << std::endl;
  std::string com("./");
  com += interface.fileName;
  com += " > /dev/null";
  EXPECT_FALSE(std::system(com.c_str()));
}
