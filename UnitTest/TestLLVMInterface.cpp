#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <forward_list>
#include <ostream>
#include <string>
#include <iostream>

#include "LLVMInterface.h"
#include "UnitTest.h"

bool TestCreatTempFile(std::forward_list<TestError*> &col);
bool TestCallLLVM(std::forward_list<TestError*> &col);
bool TestEmptySdl2App(std::forward_list<TestError*> &col);

bool TestLLVMInterface(std::forward_list<TestError*> &ret){
  bool returnValue = false;

  std::cerr << std::endl << "---------------------------------" << std::endl << "test1:" << std::endl;
  returnValue |= TestCreatTempFile(ret);
  std::cerr << std::endl << "---------------------------------" << std::endl << "test2:" << std::endl;
  returnValue |= TestCallLLVM(ret);
  std::cerr << std::endl << "---------------------------------" << std::endl << "test3:" << std::endl;
  returnValue |= TestEmptySdl2App(ret);

  return returnValue;
}

bool TestCallLLVM(std::forward_list<TestError*> &col){
  LLVMInterface interface("test.out");
  interface.CreatTempFile();

  interface.llvmFile << "#include <stdio.h>\nint main(int argc, const char *argv[]){printf(\"Hello World\\n\");}" << std::endl;

  interface.CallLLVM();

  if ( !std::filesystem::exists(interface.fileName)){
    col.push_front(new TestError(
          std::string(__func__), "The output file doesn't exist. Some thing in the compilation went wrong.", 1, 0));
    return true;
  }

  interface.llvmFile.close();

  std::string com("./");
  com += interface.fileName;
  com += " > ";
  com += interface.llvmFileName;
  std::system(com.c_str());
  /*if(std::system(com.c_str())){
    col.push_front(new TestError(
    std::string(__func__), "The hello world programm didn't finish with the exitstatus 0.", 1, 0));
    return true;
  }*/

  interface.llvmFile.open(interface.llvmFileName);

  std::ifstream t("file.txt");
  std::stringstream buffer;
  buffer << t.rdbuf();

  if(!buffer.str().compare("Hello World")){
    col.push_front(new TestError(
    std::string(__func__), "The hello world programm didn't seam to work.", 1, 0));
    return true;
  }
  
  std::remove(interface.fileName);

  return false;
}

bool TestCreatTempFile(std::forward_list<TestError*> &col){
  LLVMInterface interface;
  interface.CreatTempFile();
  std::cerr << interface.llvmFileName << std::endl;

  if ( !interface.llvmFile.good()){
    col.push_front(new TestError(
          std::string(__func__), "fstream wasn't opend therfor the file dosn't exist", 1, 0));
    return true;
  }
  return false;
}

bool TestEmptySdl2App(std::forward_list<TestError*> &col){
  LLVMInterface interface("test.out");
  interface.CreatTempFile();

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

  if ( !std::filesystem::exists(interface.fileName)){
    col.push_front(new TestError(
          std::string(__func__), "The output file doesn't exist. Some thing in the compilation went wrong.", 1, 0));
    return true;
  }

  interface.llvmFile.close();

  std::string com("./");
  com += interface.fileName;
  com += " > ";
  com += interface.llvmFileName;
  std::system(com.c_str());
  /*if(std::system(com.c_str())){
    col.push_front(new TestError(
    std::string(__func__), "The hello world programm didn't finish with the exitstatus 0.", 1, 0));
    return true;
  }*/

  interface.llvmFile.open(interface.llvmFileName);

  std::ifstream t("file.txt");
  std::stringstream buffer;
  buffer << t.rdbuf();

  if(!buffer.str().compare("Hello World")){
    col.push_front(new TestError(
    std::string(__func__), "The hello world programm didn't seam to work.", 1, 0));
    return true;
  }
  
  //std::remove(interface.fileName);

  return false;
}
