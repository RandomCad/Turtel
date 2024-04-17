#ifndef UNIT_TEST
#define UNIT_TEST

#include <string>
#include <any>
#include <iostream>

#define NOT_NULL_ASSERT(ptr, ret, num) if( ptr == nullptr){ \
                              ret = new TestError(std::string(__func__), "not nullptr assert fail.", 1, num);  \
                              return true; \
                              } 

#define NULL_ASSERT(ptr, ret, num) if( ptr != nullptr){ \
                              ret = new TestError(std::string(__func__), "nullptr assert fail.", 1, num);  \
                              return true; \
                              } 

#define STRING_ASSERT(str1, str2, ret, num) \
  if(str1.compare(str2) != 0){ \
  ret = new TestError(std::string(__func__), "String assert fail.", 1, num); return true; }
#define INT_ASSERT(int1, int2, ret, num) \
  if(int1 != int2){\
    ret = new TestError(std::string(__func__), "Int equal assert fail.", 1, num); return true; }

struct TestError {
  std::string testName;
  std::string errName;
  int sev;
  int num;
  TestError(std::string &name, std::string &err, int severity): testName(name), errName(err), sev(severity) {}
  TestError(std::string &&name, std::string &&err, int severity, int Num = -1): testName(name),
    errName(err), sev(severity), num(Num) {}
};

std::string RandomString(const char val[], const size_t len);

#endif
