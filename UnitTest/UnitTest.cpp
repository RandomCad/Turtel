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
#include <iostream>

#include "../Lexer/Lexer.h"

//specific includes:
#include "../Lexer/LexerError.h"
void TestRemoveWhites(){
  Lex st("./UnitTest/TestData/blanck.test");
  bool handeld = false;

  //testing with white file sould return a eof error since file dosn't contain non white caracters
  try{
    st.TestGetToken();
  }
  catch (EOFErr){
    handeld = true;
  }
  catch (NotImpli a){
    ERR("TestRemoveWhites", a.what()); 
    handeld = true;
  }
  catch (...){
    ERR("TestRemoveWhites", "Testing lexer for removing of whites.\n\
Error was throwen EOFErr was expected.");
    throw;
  }
  if(!handeld)
    ERR("TestRemoveWhites", "Testing lexer for removing of whites.\n\
Didn't throw EOFError as expected.");
  
  //testing with whites and no good content. Should return NotImplimented error
  handeld = false;
  Lex st2("./UnitTest/TestData/WhiteFollowedByJiber.test");
  try{
    st2.TestGetToken();
  }
  catch (NotImpli){
    handeld = true;
  }
  catch (EOFErr){
    ERR("TestRemoveWhites", "Testing lexer for removing of whites. Second test.\n\
Throw EOFError, which isn't expected"); 
  }
  catch (...){
    //TODO: Impliment
  }
  if(!handeld)
    ERR("TestRemoveWhites", "Testing lexer for removing of whites. Second test.\n\
      Didn't throw NotImpl as expected.");

  
}

void TestLexWalk(){

}

void TestLexer(){
  TestRemoveWhites();
  TestLexWalk();
}


TEST(main)

  TestLexer();

  if(!error.empty()){
    for( auto i:error){
      std::cerr << i;
    }
    
    return 1;
  }

  return 0;
}

std::ostream &operator<< (std::ostream &a, TestE &b){
  return a  << "TestMethode : " << b.meth
            << "\nTestError   : " << b.err
            << "\nSevarety    : " << b.sev << std::endl << std::endl;
}
