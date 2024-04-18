

#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(std::ostream &outStream) : output(outStream){
  ProgrammBase();
}

void CodeGenerator::ProgrammBase(){
  //Headers
  output 
    << "//Standart includes\n"
    << "#include <stdlib.h>\n"
    << "#include <math.h>\n";
  
  //Prototype generation
output
    << "//Prototypes\n";
  //main
  output
    << "//Main\n"
    << "int main(int argc, const char *argv[]){\n";
}
