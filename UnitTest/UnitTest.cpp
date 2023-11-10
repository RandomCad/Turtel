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

//Defines the TestE class which is the "Error" type to use to represent Unittest errors
//Use the below list to stor the Errors and the defines afther that to add to the list!
#include "AssertError.h"

std::forward_list<TestE> error;

//making the use of the AsserError esear:
#define Inf(meth, name) error.emplace_front( (meth), (name), Sever::Info);
#define Deb(meth, name) error.emplace_front( (meth), (name), Sever::Debug);
#define ERR(meth, name) error.emplace_front( (meth), (name), Sever::Error);
#define Fat(meth, name) error.emplace_front( (meth), (name), Sever::Fatal);

//the TestData Path is shortend for the use in this file:
#define TestD UnitTest/TestData

//General includes
#include <ostream>

#include "../Lexer/Lexer.h"


void TestRemoveWhites(){
  Lex st("./TestD");
  try{
    Token *res = st.TestGetToken();
  
    if(res != nullptr) ERR("TestRemoveWhites", "Testing lexer for removing of whites.\n\
      Expected nullptr got a real pointer");
  }
  catch (...){
    ERR("TestRemoveWhites", "Testing lexer for removing of whites.\n\
        Error was throwen no respons was expected");
  }
}

void TestLexer(){
  TestRemoveWhites();

}


TEST(main)

  TestLexer();

  if(!error.empty()){
    
    return 1;
  }

  return 0;
}

std::ostream &operator<< (std::ostream &a, TestE &b){
  return a  << "TestMethode : " << b.meth
            << "TestError   : " << b.err
            << "Sevarety    : " << b.sev;
}
