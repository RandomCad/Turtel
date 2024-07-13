#include <filesystem>
#include <forward_list>

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

