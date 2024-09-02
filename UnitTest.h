#include <cstdlib>
#include <cstring>
#include <ostream>
#ifndef UNIT_TEST
#pragma once

#define TEST_OUTPUT_DIR ./Testing/Temporary/

#include <string>

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
#define REGEX_ASSERT(str, regStr, ret, num) \
  if(!std::regex_match(str, std::regex(regStr, std::regex_constants::ECMAScript))){\
    ret = new TestError(__func__, "regex didn't match: " regStr, 1,num);\
    return true;\
  }
    

enum TestErrorSeveraty{
  INFO = 0,
  WARNING = 1,
  ERROR = 2,
};

#define  declareError(name, description, severity)
struct TestError {
  char *testName;
  char *errName;
  char *errDescription;
  TestErrorSeveraty severity;
  int lineNum;
  TestError(const char *name,const char *errorName, const char * desc, TestErrorSeveraty sev, int line);
  TestError(std::string name, std::string err, int severity, int Num = 1){}
  TestError(const char * name, const char * err, int severity, int testNum){}
};

std::ostream &operator<<(std::ostream &a, TestError &b);

std::string RandomString(const char val[], const size_t len);

#endif
