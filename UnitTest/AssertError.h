#ifndef ASSERT_ERR
#define ASSERT_ERR

#include <ostream>

enum Sever{
  Info,
  Debug,
  Error,
  Fatal
};

class TestE;

std::ostream &operator<< (std::ostream &a, TestE &b);

class TestE{
  private:
    std::string meth;
    std::string err;
    Sever sev;
  public:
    TestE(std::string Meth, std::string Err, Sever Sev = Sever::Error):
      meth(Meth), err(Err), sev(Sev) {}

    friend std::ostream &operator<< (std::ostream &a, TestE &b);
};

#endif
