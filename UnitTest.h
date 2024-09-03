#include <cstdlib>
#include <cstring>
#include <ostream>
#ifndef UNIT_TEST
#pragma once

#define TEST_OUTPUT_DIR ./Testing/Temporary/

#include <string>

#define NOT_NULL_ASSERT(ptr, ret, num) if( ptr == nullptr){ \
                              ret = declareError("NotNullAssert", "not nullptr assert failed.", TestErrorSeveraty::ERROR);  \
                              return true; \
                              } 

#define NULL_ASSERT(ptr, ret, num) if( ptr != nullptr){ \
                              ret = declareError("NullAssert", "nullptr assert failed.", TestErrorSeveraty::ERROR);  \
                              return true; \
                              } 

#define STRING_ASSERT(str1, str2, ret, num) \
  if(str1.compare(str2) != 0){ \
  ret = declareError("StringAssert", "string assert failed.", TestErrorSeveraty::ERROR);  \
  return true; }

#define INT_ASSERT(int1, int2, ret, num) \
  if(int1 != int2){\
  ret = declareError("IntAssert", "int assert failed.", TestErrorSeveraty::ERROR);  \
  return true; }

#define REGEX_ASSERT(str, regStr, ret, num) \
  if(!std::regex_match(str, std::regex(regStr, std::regex_constants::ECMAScript))){\
  ret = declareError("RegexAssert", "regex assert failed.", TestErrorSeveraty::ERROR);  \
    return true;\
  }

#define TRUE_ASSERT(exp, col, sev) \
  if(!exp){\
    col = declareError("TrueAssert", "the expresion wasn't true", sev); \
    return true;\
  }
    

enum TestErrorSeveraty{
  INFO = 1,
  WARNING = 2,
  ERROR = 3,
};

#define  declareError(name, description, severity) new TestError("__func__", name, description, severity, __LINE__)
struct TestError {
  const char *testName;
  const char *errName;
  const char *errDescription;
  TestErrorSeveraty severity;
  int lineNum;
  TestError(const char *name,const char *errorName, const char * desc, TestErrorSeveraty sev, int line);
  //TestError(std::string name, std::string err, int severity, int Num = 1){ throw "not suported";}
  //TestError(const char * name, const char * err, int severity, int testNum){ throw "not suported";}
};

std::ostream &operator<<(std::ostream &a, TestError &b);

std::string RandomString(const char val[], const size_t len);

#endif
