#include "src/CommandLineListener.h"

void CommandLineListener::exitArgs(CommandlineParser::ArgsContext *ctx){
  opt.inputFile = ctx->FileName(1)->getText();
}
void CommandLineListener::exitOutput(CommandlineParser::OutputContext * ctx){
  opt.outputFile = ctx->FileName()->getText();
}
