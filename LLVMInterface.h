#ifndef LLVMINTERVACE_FILE
#define LLVMINTERVACE_FILE

#include <cstddef>
#include <cstring>
#include <fstream>
#include <forward_list>
#include <vector>

#include "UnitTest.h"

#define NameLength 32
class LLVMInterface{
  char llvmFileName[NameLength];
  int fileDescriptor;
  const char * fileName;
  const size_t fileNameLen;
  std::vector<std::string> sdlArgs;

  LLVMInterface() : fileName(nullptr) , fileNameLen(0) {}

  public:
    std::fstream llvmFile;
    
    ~LLVMInterface();
    LLVMInterface(const char *const FileName) : fileName(new char[std::strlen(FileName)]) , fileNameLen(std::strlen(FileName)){
      std::memcpy(const_cast<char*>(fileName), FileName, fileNameLen);
      CreatTempFile();
    }
    
    void CallLLVM();
  private:
    void CreatTempFile();
    void getSdlArguments();
    friend bool TestCreatTempFile(TestError*);
    friend bool TestCallLLVM(TestError*);
    friend bool TestEmptySdl2App(TestError*);
};

#endif
