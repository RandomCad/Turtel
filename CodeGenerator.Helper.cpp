#include "CodeGenerator.h"
#include <fstream>

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

void CodeGenerator::EndeMain(){
  output 
    << "//End of Main\n"
    << "}\n//Implimentation start for funktions";
}


