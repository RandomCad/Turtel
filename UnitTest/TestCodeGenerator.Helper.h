#ifndef TEST_CODEGEN_HELPER
#define TEST_CODEGEN_HELPER

#include "../UnitTest.h"
#include <stack>

bool TestCodeGeneratorProgramBase(TestError *&);
bool TestCodeGeneratorEndMain(TestError *&);
bool TestCodeGeneratorCTor(TestError *&);
bool TestCodeGeneratorDTor(TestError *&);
bool TestCodeGeneratorCreatTempFile(TestError *&);
bool TestCodeGeneratorEmpty(TestError *&);
bool TestCodeGenerator(std::stack<TestError*>&);


#endif
