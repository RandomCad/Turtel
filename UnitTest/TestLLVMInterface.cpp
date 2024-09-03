#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <ostream>
#include <string>
#include <iostream>

#include "LLVMInterface.h"
#include "UnitTest.h"
#include "TestLLVMInterface.h"

bool TestCreatTempFile(TestError*&);
bool TestCallLLVM(TestError*&);
bool TestEmptySdl2App(TestError*&);

bool TestLLVMInterface(std::stack<TestError*> &ret){
  bool returnValue = false;

  TestError * in;
  std::cerr << std::endl << "---------------------------------" << std::endl << "test1:" << std::endl;
  if(TestCreatTempFile(in)){
    ret.push(in);
    returnValue = true;
  }
  std::cerr << std::endl << "---------------------------------" << std::endl << "test2:" << std::endl;
  if(TestCallLLVM(in)){
    ret.push(in);
    returnValue = true;
  }
  std::cerr << std::endl << "---------------------------------" << std::endl << "test3:" << std::endl;
  if(TestEmptySdl2App(in)){
    ret.push(in);
    returnValue = true;
  }

  return returnValue;
}

bool TestCallLLVM(TestError* &col){
  LLVMInterface interface("test.out");

  interface.llvmFile << "#include <stdio.h>\nint main(int argc, const char *argv[]){printf(\"Hello World\\n\");}" << std::endl;

  interface.CallLLVM();

  TRUE_ASSERT(!std::filesystem::exists(interface.fileName), col, TestErrorSeveraty::ERROR);

  interface.llvmFile.close();

  std::string com("./");
  com += interface.fileName;
  com += " > ";
  com += interface.llvmFileName;
  std::system(com.c_str());

  interface.llvmFile.open(interface.llvmFileName);

  std::ifstream t("file.txt");
  std::stringstream buffer;
  buffer << t.rdbuf();

  STRING_ASSERT(buffer.str(), "Hello World", col, 0);
  
  std::remove(interface.fileName);

  return false;
}

bool TestCreatTempFile(TestError* &col){
  LLVMInterface interface;
  interface.CreatTempFile();
  std::cerr << interface.llvmFileName << std::endl;

  TRUE_ASSERT(interface.llvmFile.good(), col, TestErrorSeveraty::ERROR)
  return false;
}

bool TestEmptySdl2App(TestError* &col){
  LLVMInterface interface("test.out");

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

  TRUE_ASSERT(std::filesystem::exists(interface.fileName), col, TestErrorSeveraty::ERROR);

  interface.llvmFile.close();

  std::string com("./");
  com += interface.fileName;
  com += " > ";
  com += interface.llvmFileName;
  std::system(com.c_str());

  interface.llvmFile.open(interface.llvmFileName);

  std::ifstream t("file.txt");
  std::stringstream buffer;
  buffer << t.rdbuf();

  STRING_ASSERT(buffer.str(), "Hello World", col, 1);
  
  std::remove(interface.fileName);

  return false;
}
