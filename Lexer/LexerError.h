#ifndef LEXER_ERROR
#define LEXER_ERROR

#include <exception>
#include <queue>

class LexErr: public std::exception{
  private:
    std::string sb;
    const int startSize = 16;
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

    std::queue<Error> error;
  public:
    enum class Status{ OK,WARN,ERROR,FATAL};
    Status state;

    //Ctor
    LexErr() : state(Status::OK){
      sb.reserve(startSize);
    }
    //TODO: Detor
    //
    //Its more of a warning than a real error!
    LexErr &TryConsumedWrong(char corr){
      Error::S zwi;
      zwi.car = corr;
      error.push(Error(Error::errorNum::TryWrongConsum,zwi, sb.length() - 1));
      return *this;
    }
       
    void WalkLexingError(){
      Error::S zwi = {nullptr};
      error.push(Error(Error::errorNum::WalkLexErr,zwi, sb.length() - 1));
      state = Status::OK;
    }

    LexErr &ConsumedWrong(char corr){
      Error::S zwi;
      zwi.car = corr;
      error.push(Error(Error::errorNum::WrongConsum,zwi, sb.length() - 1));
      state = (state == Status::OK) ? Status::WARN : state;
      return *this;
    }
    void EOFReached(){
      state = Status::FATAL;
    } 
    void AddChar(const char a) noexcept{
      sb += a;
    }
  private:

};

class EOFErr : public LexErr{
  public:
    const char * what(){
      return "Encounterd end of file, whene this wasn't expected";
    }
};

//TODO save what is wrong
class NotImpli : public LexErr{
  private:
    const char wrong;
    std::string ret = "Encounterd symbole { } for which no lexing path was founde";
  public:
    NotImpli(const char a) : wrong(a) {}
    const char * what(){
      ret[20] = wrong;
      return ret.c_str();
    }
};

#endif
