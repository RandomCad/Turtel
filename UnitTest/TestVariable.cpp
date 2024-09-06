#include "TestVariable.h"
#include "../src/Variable.h"
#include "UnitTest.h"

bool TestVariable(std::stack<TestError *> &col){
  Variable test(RENDERER, "test");
  if(!test.name.compare("test")){
    col.push( declareError("TestVariableNameChanged", "The variable name changed", TestErrorSeveraty::ERROR ));
  }
  if(!test.getName().compare("test")) col.push( declareError("TestVariableNameChanged", "The variable name changed wenn it was extracted", TestErrorSeveraty::ERROR ));
  if(!test.getTypeAndName().compare("SDL_Renderer * test")) col.push( declareError("TestVariableNameChanged", "The variable name changed wenn it was extracted", TestErrorSeveraty::ERROR ));

  return true;
}
