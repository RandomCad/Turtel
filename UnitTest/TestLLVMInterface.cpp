#include "LLVMInterface.h"
#include "UnitTest.h"

bool TestCreatTempFile(TestError *&col);

bool TestLLVMIntervace(TestError *&ret){
  bool returnValue = false;

  returnValue |= TestCreatTempFile(ret);

  return returnValue;
}

bool TestCreatTempFile(TestError *&col){
  LLVMInterface interface;
  interface.CreatTempFile();
  std::cerr << interface.llvmFileName << std::endl;

  if ( !interface.llvmFile.good()){
    col = new TestError(std::string(__func__), "fstream wasn't opend therfor the file dosn't exist", 1, 0);
    return true;
  }
  return false;
}

