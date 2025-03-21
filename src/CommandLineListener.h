#include "libs/CommandlineBaseListener.h"
namespace Comandline {
  struct options{
    std::string inputFile;
    std::string outputFile = "out";
  };
};

class CommandLineListener : public CommandlineBaseListener{
  private:
    Comandline::options opt;
  public:
    Comandline::options GetOptions() { return opt; } 
  private:
    void exitArgs(CommandlineParser::ArgsContext *) override;

};
