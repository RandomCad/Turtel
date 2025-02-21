#include "LLVMInterface.h"

#include <alloca.h>
#include <cstdlib>
#include <cstring>
#include <cwctype>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/VirtualFileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/Program.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <clang/Driver/Job.h>
#include <clang/Driver/Driver.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Driver/Compilation.h>

//std::vector<std::string> LLVMInterface::sdlArgs;

bool CreatTempFile(char *templateStr, std::fstream &ret){
  int fd = mkstemp(templateStr);

  if (fd == -1){
    return true;
  }

  ret.open(templateStr);
  return false;

}

void LLVMInterface::CreatTempFile(){
  const char *templateForFile ="/tmp/TurtelCOutXXXXXX";
  std::strcpy(llvmFileName, templateForFile);
  if(::CreatTempFile(llvmFileName, llvmFile)){
    std::cerr << "Couldn't open output file for c intermidiate retrying" << errno << std::endl;
    return CreatTempFile();
  }
}

LLVMInterface::~LLVMInterface(){
  llvmFile.close();
  std::remove(llvmFileName);
  fileDescriptor = 0;
}

void LLVMInterface::CallLLVM(){
  auto sdlArgs = LLVMInterface::getSdlArguments();
  
	
	// Path to clang (e.g. /usr/local/bin/clang)
  auto clangPath = llvm::sys::findProgramByName("clang");
	
	// Arguments to pass to the clang driver:
	//	clang -o <output> -x c <inputFile> <SDLArgs> -v -Werror
  std::vector<const char *> args;
	args.push_back(clangPath->c_str());
	args.push_back("-o");
	args.push_back(fileName);
	args.push_back("-x");
	args.push_back("c");
	args.push_back(llvmFileName);
  for(int i = 0; i < sdlArgs.size(); ++i){
    std::cerr << sdlArgs[i] << std::endl;
    args.push_back(sdlArgs[i].c_str());
  }//*/
  //args.push_back("-l");
	//args.push_back("curl");
	args.push_back("-v");		// verbose
	args.push_back("-Werror");		// warnings as errors
  args.push_back("-lm"); ///< add the math lib to the linking stage
  args.push_back("-lSDL2_image"); ///< add link to sdl_immage
  args.push_back("-O3");
#ifndef NDEBUG
  args.push_back("-ggdb");
#endif

  for(auto i: args){
    std::cout << i << std::endl;
  }
	
	// The clang driver needs a DiagnosticsEngine so it can report problems
	//clang::TextDiagnosticPrinter *DiagClient = new clang::TextDiagnosticPrinter(llvm::errs(),&diagOpt);
	clang::DiagnosticsEngine Diags(new clang::DiagnosticIDs(), new clang::DiagnosticOptions());
	
  std::cerr << "Create" << std::endl;
	// Create the clang driver
	clang::driver::Driver TheDriver(args[0], llvm::sys::getDefaultTargetTriple(), Diags);
	
	// If you want to build C++ instead of C
	//	TheDriver.CCCIsCXX = true;
	
  
  std::cerr << "C" << std::endl;
	// Create the set of actions to perform
  std::unique_ptr<clang::driver::Compilation> C(TheDriver.BuildCompilation(args));
	
  std::cerr << "Print" << std::endl;
	// Print the set of actions
	TheDriver.PrintActions(*C);
	
  std::cerr << "PrepExecute" << std::endl;
	// Carry out the actions
	int Res = 0;
  llvm::SmallVector<std::pair<int, const clang::driver::Command*>> t;

	
  std::cerr << "Execute" << std::endl;
  if (C) Res = TheDriver.ExecuteCompilation(*C, t);
	
	// Report problems
	if (Res < 0){
	  clang::driver::Command *FailingCommand = 0;
		TheDriver.generateCompilationDiagnostics(*C, *FailingCommand);
  }
}

//static bool wasUsed = false;
std::vector<std::string> LLVMInterface::getSdlArguments(){
  static std::vector<std::string> sdlArgs;
  static bool gotArgs = 0;
  if(gotArgs){
    return sdlArgs;
  }

  char *templatFile = (char *)alloca(sizeof("/tmp/XXXXXX"));
  memcpy(templatFile, "/tmp/XXXXXX", sizeof("/tmp/XXXXXX"));
  std::fstream in;
  ::CreatTempFile(templatFile, in);
  std::string systemCommand("sdl2-config --cflags --libs >");
  systemCommand += templatFile;

  std::system(systemCommand.c_str());

  std::cout << "SDL2 configs:" << std::endl;
  for (std::string line;std::getline(in,line);){
    std::cout << line << std::endl;
    char * opt = (char *)alloca(line.length());
    size_t j=0;
    for(const char *i = line.c_str();*i;++i, ++j){
      if(std::iswspace(*i)){
        opt[j] = '\0';
        sdlArgs.push_back(std::string(opt));
        j= -1;
      }
      else opt[j] = *i;
    }
    opt[j] = '\0';
    sdlArgs.push_back(std::string(opt));
  }
  
  gotArgs = true;
  return sdlArgs;
}


