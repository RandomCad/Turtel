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
  std::fstream llvmFile;
  char llvmFileName[NameLength];
  int fileDescriptor;
  const char * fileName;
  const size_t fileNameLen;
  std::vector<std::string> sdlArgs;

  LLVMInterface() : fileName(nullptr) , fileNameLen(0) {}

  public:
    ~LLVMInterface();
    LLVMInterface(const char *const FileName) : fileName(new char[std::strlen(FileName)]) , fileNameLen(std::strlen(FileName)){
      std::memcpy(const_cast<char*>(fileName), FileName, fileNameLen);
    }
  private:
    void CreatTempFile();
    void CallLLVM();
    void getSdlArguments();
    friend bool TestCreatTempFile(std::forward_list<TestError*> &col);
    friend bool TestCallLLVM(std::forward_list<TestError*> &col);
    friend bool TestEmptySdl2App(std::forward_list<TestError*> &col);
};

#endif
