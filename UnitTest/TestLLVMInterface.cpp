#include <forward_list>

#include "LLVMInterface.h"
#include "UnitTest.h"

bool TestCreatTempFile(std::forward_list<TestError*> &col);

bool TestLLVMInterface(std::forward_list<TestError*> &ret){
  bool returnValue = false;

  returnValue |= TestCreatTempFile(ret);

  std::cerr << "OK";

  return returnValue;
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

