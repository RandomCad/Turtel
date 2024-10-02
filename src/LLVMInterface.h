#ifndef LLVMINTERVACE_FILE
#define LLVMINTERVACE_FILE

#include <cstddef>
#include <cstring>
#include <fstream>
#include <vector>

#include "../UnitTest/UnitTest.h"

#define NameLength 32
/**
 * The class that interfaces with clang.
 * It generates all the files, that should be used for the generate code of the transpiler and compiler steps.
 */
class LLVMInterface{
  char llvmFileName[NameLength];
  int fileDescriptor;
  const char * fileName;
  const size_t fileNameLen;
  ///The arguments, that are needed to compile with SDL2
  std::vector<std::string> sdlArgs;

  LLVMInterface() : fileName(nullptr) , fileNameLen(0) {}

  public:
    ///The file to put the transpieled C-Code in.
    std::fstream llvmFile;
    
    ~LLVMInterface();
    LLVMInterface(const char *const FileName) : fileName(new char[std::strlen(FileName) + 1]) , fileNameLen(std::strlen(FileName) + 1){
      std::memcpy(const_cast<char*>(fileName), FileName, fileNameLen);
      CreatTempFile();
    }
    
    ///This function call compiles the llvmFile witch clang.
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
