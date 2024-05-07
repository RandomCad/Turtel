#ifndef LLVMINTERVACE_FILE
#define LLVMINTERVACE_FILE

#include <fstream>
#include "UnitTest.h"
class LLVMInterface{
  std::fstream llvmFile;
  char *llvmFileName;
  int fileDescriptor;

  public:
    ~LLVMInterface(){
      llvmFile.close();
      std::remove(llvmFileName);
      fileDescriptor = 0;
    }

    void CreatTempFile();
    friend bool TestCreatTempFile(TestError *&);
};

#endif
