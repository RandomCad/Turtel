#include "CommandlineParser.h"
#include "libs/CommandlineBaseListener.h"
#include <vector>
namespace Comandline {
  struct options{
    std::string inputFile;
    std::string outputFile = "out";
    bool interpretationMode = false;
    int infinitLoop = 0;
    std::vector<std::string> turtelArgs;
  };
};

class CommandLineListener : public CommandlineBaseListener{
  private:
    bool infLoopDesieded = false;
    Comandline::options opt;
  public:
    Comandline::options GetOptions() { return opt; } 
  private:
    void exitArgs(CommandlineParser::ArgsContext *) override;
    void exitOutput(CommandlineParser::OutputContext *) override;
    void exitInterp(CommandlineParser::InterpContext *) override;
    void exitAllowInf(CommandlineParser::AllowInfContext *) override;
    void exitDisalInf(CommandlineParser::DisalInfContext *) override;
    void exitTurtelArg(CommandlineParser::TurtelArgContext *) override;

};
