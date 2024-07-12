#ifndef LLVMINTERVACE_FILE
#define LLVMINTERVACE_FILE

#include <fstream>
#include <forward_list>

#include "UnitTest.h"

#define NameLength 32
class LLVMInterface{
  std::fstream llvmFile;
  char llvmFileName[NameLength];
  int fileDescriptor;

  public:
    ~LLVMInterface();
    LLVMInterface();

    void CreatTempFile();
    void CallLLVM();
    friend bool TestCreatTempFile(std::forward_list<TestError*> &col);
};

#endif
