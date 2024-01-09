#ifndef LEXER_ERROR
#define LEXER_ERROR

#include <exception>
#include <queue>
#include <fstream>
#include <string>

class LexErr;
std::ostream &operator<< (std::ostream &a, LexErr &b);

class LexErr: public std::exception{
  private:
    struct Error{
       enum class errorNum {
        EndOfFile,
        WrongConsum,
        TryWrongConsum,
        WalkLexErr //it's a error that specifies errors with already happend
      };
      union S{
        void *vp;
        char car;
      };
      errorNum err;
      int sbPos;
      S content;

      Error(errorNum e, S c,int s) : err(e),sbPos(s), content(c) {}
    };

    std::string sb;
    const int startSize = 16;
    std::queue<Error> error;
  public:
    friend std::ostream &operator<< (std::ostream &a, LexErr &b);
    enum class Status{ OK,WARN,ERROR,FATAL};
    Status state;

    //Ctor
    LexErr() : state(Status::OK){
      sb.reserve(startSize);
    }
    //TODO: Detor
    
    void ResetWarn(){
      switch (state){
        case Status::OK:
        case Status::WARN:
          state = Status::OK;
        case Status::ERROR:
        case Status::FATAL:
          break;
      }
    }
    void ResetError(){
      switch (state){
        case Status::OK:
        case Status::WARN:
        case Status::ERROR:
          state = Status::OK;
        case Status::FATAL:
          break;
      }
    }
    //Its more of a warning than a real error!
    LexErr &TryConsumedWrong(char corr);
    void AddWalkLexingError();
    LexErr &ConsumedWrong(char corr);


    void EOFReached() noexcept{
      state = Status::FATAL;
    } 
    void AddChar(const char a) noexcept{
      sb += a;
    }
  private:
    static std::ostream &PrintError(std::ostream &a, Error &err);

};

#endif
