#include "LLVMInterface.h"
#include <cstring>
#include <vector>

#include <llvm/Support/VirtualFileSystem.h>
#include <clang/Driver/Driver.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/Program.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>

void LLVMInterface::CreatTempFile(){
  const char *templateForFile ="/tmp/TurtelCOutXXXXXX";
  std::strcpy(llvmFileName, templateForFile);
  std::cerr << "OK" << std::endl;
  int fd = mkstemp(llvmFileName);

  if (fd == -1){
    std::cerr << "Couldn't open output file for c intermidiate retrying" << errno << std::endl;
    return CreatTempFile();
  }

  llvmFile.open(llvmFileName);
}

LLVMInterface::~LLVMInterface(){
  llvmFile.close();
  std::remove(llvmFileName);
  fileDescriptor = 0;
}

void LLVMInterface::CallLLVM(){
  
	
	// Path to clang (e.g. /usr/local/bin/clang)
  auto clangPath = llvm::sys::findProgramByName("clang");
	
	// Arguments to pass to the clang driver:
	//	clang getinmemory.c -lcurl -v
  std::vector<const char *> args;
	args.push_back(clangPath->c_str());
	args.push_back(llvmFileName);
	args.push_back("-l");
	args.push_back("curl");
	args.push_back("-v");		// verbose
	
	// The clang driver needs a DiagnosticsEngine so it can report problems
  clang::DiagnosticOptions *diagOpt = new clang::DiagnosticOptions();
	clang::IntrusiveRefCntPtr<clang::DiagnosticOptions> DiagOpt(diagOpt);
	clang::TextDiagnosticPrinter *DiagClient = new clang::TextDiagnosticPrinter(llvm::errs(),diagOpt);
	clang::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagID(new clang::DiagnosticIDs());
	clang::DiagnosticsEngine Diags(DiagID, DiagOpt);
	
	// Create the clang driver
	clang::driver::Driver TheDriver(args[0], llvm::sys::getDefaultTargetTriple(), Diags);
	
	// If you want to build C++ instead of C
	//	TheDriver.CCCIsCXX = true;
	
  /*
	// Create the set of actions to perform
	clang::OwningPtr<clang::driver::Compilation> C(TheDriver.BuildCompilation(args));
	
	// Print the set of actions
	TheDriver.PrintActions(*C);
	
	// Carry out the actions
	int Res = 0;
	const clang::driver::Command *FailingCommand = 0;
	if (C)
		Res = TheDriver.ExecuteCompilation(*C, FailingCommand);
	
	// Report problems
	if (Res < 0)
		TheDriver.generateCompilationDiagnostics(*C, FailingCommand);
*/

}

LLVMInterface::LLVMInterface(){
  
}
