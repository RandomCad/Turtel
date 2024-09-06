#ifndef LLVMINTERVACE_FILE
#define LLVMINTERVACE_FILE

#include <cstddef>
#include <cstring>
#include <fstream>
#include <vector>

#include "../UnitTest/UnitTest.h"

#define NameLength 32
class LLVMInterface{
  char llvmFileName[NameLength];
  int fileDescriptor;
  const char * fileName;
  const size_t fileNameLen;

  LLVMInterface() : fileName(nullptr) , fileNameLen(0) {}

  public:
    std::fstream llvmFile;
    
    ~LLVMInterface();
    LLVMInterface(const char *const FileName) : fileName(new char[std::strlen(FileName) + 1]) , fileNameLen(std::strlen(FileName) + 1){
      std::memcpy(const_cast<char*>(fileName), FileName, fileNameLen);
      CreatTempFile();
    }
    
    void CallLLVM();
  private:
    void CreatTempFile();
    std::vector<std::string> getSdlArguments();

#ifdef UNIT_TEST
    friend bool TestCreatTempFile(TestError*&);
    friend bool TestCallLLVM(TestError*&);
    friend bool TestEmptySdl2App(TestError*&);
#endif
};

#endif
