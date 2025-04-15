#include "src/CommandLineListener.h"

/**
 * @brief Handles the completion of command-line argument parsing.
 *
 * Sets the input file option based on the parsed command-line arguments.
 *
 * @param ctx Pointer to the context containing the parsed arguments.
 */
void CommandLineListener::exitArgs(CommandlineParser::ArgsContext *ctx){
  opt.inputFile = ctx->FileName(1)->getText();
}

/**
 * @brief Handles the completion of output file parsing.
 *
 * Sets the output file option based on the parsed output command.
 *
 * @param ctx Pointer to the context containing the parsed output command.
 */
void CommandLineListener::exitOutput(CommandlineParser::OutputContext * ctx){
  opt.outputFile = ctx->FileName()->getText();
}
