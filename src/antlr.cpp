#include <ANTLRInputStream.h>
#include <any>
#include <ios>
#include <istream>
#include <sstream>
#include <streambuf>

#include "libs/SceneLexer.h"
#include "libs/SceneParser.h"
#include "libs/CommandlineLexer.h"
#include "libs/CommandlineParser.h"
#include "src/TopLevelVisitor.h"
#include "src/CommandLineListener.h"

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
  std::stringstream argvStream;
  for (const char **i = argv; *i; ++i){
    argvStream << *i << ' ';
  }

  antlr4::ANTLRInputStream argInput(argvStream);
  CommandlineLexer argLex(&argInput);
  antlr4::CommonTokenStream argTokens(&argLex);
  CommandlineParser argPars(&argTokens);

  CommandLineListener argLis;
  argPars.addParseListener(&argLis);

  argPars.args();
  
  Comandline::options opt = argLis.GetOptions();

  std::filebuf fb;
  if(!fb.open(opt.inputFile, std::ios::in)){
    throw  "error"; //TODO;
  }

  std::istream turtelFile(&fb);
  antlr4::ANTLRInputStream input(turtelFile);
  SceneLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  SceneParser parser(&tokens);    

  SceneParser::FileContext *file = parser.file();

  TopLevelVisitor vis(opt.outputFile.c_str());

  try {
    file->accept(&vis);
  } catch (std::bad_any_cast &a) {
    std::cerr << a.what() << std::endl ;
    throw a;
  
  }

  return 0;
}
