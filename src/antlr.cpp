#include <ANTLRInputStream.h>
#include <ios>
#include <istream>
#include <streambuf>

#include "libs/SceneLexer.h"
#include "libs/SceneParser.h"
#include "src/TopLevelVisitor.h"

/**
 * @param argv hase the formate Turtel [Options] <turtel-file> [Options] [ -- [Turtel-Options]] 
 *        Options:
 *        * --output <file-name>  : sets the name of the executable
 *        * --interpret           : activates the interpretation mode -> the generated executable is deledet at programme end
 *        * --allow-infinitloop   : explisitly allow infinitloops -> no warnings are generated
 *        * --disallow-infinitloop: explizitly disalow infinitloops -> infinitloops alwas generate an error
 **/
#ifndef UNIT_TEST
int main(int argc, const char* argv[]) {
#else
int progMain(int argc, const char *argv[]){
#endif
  const char *inputFile;
  bool foundFile = false;
  for (const char *i = *argv + 2; i; ++i) {
    if(!i[0]) continue;
    else if(i[0] == '-'){
      if(i[0] != '-') throw "error"; //TODO
      else {

      }
    }
    else{
      if(!foundFile) {
        inputFile = i;
        foundFile = true;
      }
      else throw "error"; //TODO
    }
  }

  std::filebuf fb;
  if(!fb.open(inputFile, std::ios::in)){
    throw  "error"; //TODO;
  }

  std::istream turtelFile(&fb);
  antlr4::ANTLRInputStream input(turtelFile);
  SceneLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    

  SceneParser::FileContext *file = parser.file();

  TopLevelVisitor vis;

  file->accept(&vis);

  return 0;
}
