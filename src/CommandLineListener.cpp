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

/**
 * If this rule is enterd the compiler should be used as interpreter.
 * All the generated files must be deleted.
 */
void CommandLineListener::exitInterp(CommandlineParser::InterpContext *){
  opt.interpretationMode = true;
}

/**
 * Explizitaly allows infinit loops in the code
 */
void CommandLineListener::exitAllowInf(CommandlineParser::AllowInfContext *){
  if(infLoopDesieded){
    std::cout << "You already used one of the flags `--allow-infinitloop` ore `--disallow-infinitloop`.\nOnly use one of thes flags. The last one will be aplied for now." << std::endl;
  }
  infLoopDesieded = true;
  opt.infinitLoop = 1;
}
/**
 * Explizitaly disallows infinit loops in the code
 */
void CommandLineListener::exitDisalInf(CommandlineParser::DisalInfContext *){
  if(infLoopDesieded){
    std::cout << "You already used one of the flags `--allow-infinitloop` ore `--disallow-infinitloop`.\nOnly use one of thes flags. The last one will be aplied for now." << std::endl;
  }
  infLoopDesieded = true;
  opt.infinitLoop = -1;
}
/**
 * Parses all the arguments for the interpreter from the CLI
 */
void CommandLineListener::exitTurtelArg(CommandlineParser::TurtelArgContext *ctx){
  for (auto i : ctx->Float()) {
    opt.turtelArgs.push_back(i->getText());
  }
}
