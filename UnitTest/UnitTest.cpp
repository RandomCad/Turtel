#ifndef U_TEST
#error "UnitTest should be compiled with U_TEST"
#endif

//this is set up for automatick testing.
//each test should cal the prefius test
//Use the folowing makro for this!

#define TEST(name) int name(){  int ret = PREF;  if(ret != 0) return ret;  

//Afther the macro you moust redifine PREF as Follows:
// #define PREF name()

#define PREF 0

#include <forward_list>
#include <ostream>

#include "AssertError.h"

std::forward_list<TestE> error;

//making the use of the AsserError esear:
#define Inf(meth, name) error.emplace_back( (meth), (name), Sever::Info);
#define Deb(meth, name) error.emplace_back( (meth), (name), Sever::Debug);
#define ERR(meth, name) error.emplace_back( (meth), (name), Sever::Error);
#define Fat(meth, name) error.emplace_back( (meth), (name), Sever::Fatal);

void TestWalk(){

}

void TestLexer(){

}


TEST(main)

  TestLexer();

  return 0;
}

std::ostream &operator<< (std::ostream &a, TestE &b){
  return a  << "TestMethode : " << b.meth
            << "TestError   : " << b.err
            << "Sevarety    : " << b.sev;
}
