#ifndef LLVMINTERVACE_FILE
#define LLVMINTERVACE_FILE

#include <fstream>
#include "UnitTest.h"
class LLVMInterface{
  std::fstream llvmFile;
  char *llvmFileName;

  public:
    ~LLVMInterface(){
      llvmFile.close();
      std::remove(llvmFileName);
      free(llvmFileName);
    }

    void CreatTempFile();
    friend bool TestCodeGeneratorCreatTempFile(TestError *&);
};

#endif
