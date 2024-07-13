#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <forward_list>
#include <string>
#include <iostream>

#include "LLVMInterface.h"
#include "UnitTest.h"

bool TestCreatTempFile(std::forward_list<TestError*> &col);
bool TestCallLLVM(std::forward_list<TestError*> &col);

bool TestLLVMInterface(std::forward_list<TestError*> &ret){
  bool returnValue = false;

  returnValue |= TestCreatTempFile(ret);
  returnValue |= TestCallLLVM(ret);

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

