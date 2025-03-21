#include "src/CommandLineListener.h"

void CommandLineListener::exitArgs(CommandlineParser::ArgsContext *ctx){
  opt.inputFile = ctx->FileName(1)->getText();
}
