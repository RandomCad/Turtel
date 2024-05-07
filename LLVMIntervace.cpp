#include "LLVMInterface.h"
#include <cstring>
void LLVMInterface::CreatTempFile(){
  const char *templateForFile ="/tmp/TurtelCOutXXXXXX";
  std::strcpy(llvmFileName, templateForFile);
  int fd = mkstemp(llvmFileName);

  if (fd == -1){
    std::cerr << "Couldn't open output file for c intermidiate retrying" << errno << std::endl;
    return CreatTempFile();
  }

  llvmFile.open(llvmFileName);
}
