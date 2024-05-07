#include "LLVMInterface.h"
#include <cstring>
void LLVMInterface::CreatTempFile(){
  const char *templateForFile ="/run/TurtelCOutXXXXXX";
  std::strcpy(llvmFileName, templateForFile);
  int res = mkstemp(llvmFileName);

  if (res == -1){
    std::cerr << "Couldn't open output file for c intermidiate retrying" << std::endl;
    return CreatTempFile();
  }

  fclose((FILE *)res);

}
