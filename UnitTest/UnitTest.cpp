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
#include "../Lexer/LexerError.h"

/*
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
    st2.GetToken();
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

  
}*/

void TestLexWalk(){
  Lex st("./UnitTest/TestData/Walk.test");
  LexErr err;
  auto ret = st.RWalk(err);
  std::cerr << err << std::endl;
  if (err.state != LexErr::Status::OK) ERR("TestLexWall","Testing the corect lexing of walk, wrong err state");
  std::cerr << ret << ' ' <<(err.state != LexErr::Status::OK) << std::endl;
  if (ret->tok != TokE::Walk) ERR("TestLexWall","Testing the corect lexing of walk wrong token");

  err.state = LexErr::Status::OK;
  ret = st.RWalk(err);
  if (err.state != LexErr::Status::OK) ERR("TestLexWall","Testing the corect lexing of walk, wrong err state");
  if (ret->tok != TokE::Walk) ERR("TestLexWall","Testing the corect lexing of walk wrong token");

  err.state = LexErr::Status::OK;
  ret = st.RWalk(err);
  if (err.state != LexErr::Status::OK) ERR("TestLexWall","Testing the incorect lexing of walk, wrong err state");
  if (ret == nullptr) ERR("TestLexWall","Testing the incorect lexing of walk a token was lexed");
  //TODO: check the error list
 

}

#define ConsumeTest(data, num) err.state=LexErr::Status::OK;if(st.ConsumTerm( data , err).state != LexErr::Status::OK) ERR("TestConsumTerm", "Testing the consum therm method faild to consum "#num) 
#define nConsumeTest(data, num) err.state=LexErr::Status::OK;if(st.ConsumTerm( data , err).state != LexErr::Status::WARN) ERR("TestConsumTerm", "Testing the consum therm method faild it wrongly consumed "#num" "#data) 
void TestConsumTerm(){
  Lex st("./UnitTest/TestData/RandomContent.test");
  LexErr err;
  nConsumeTest('b',1);
  ConsumeTest('q',2);
  ConsumeTest('6',3);
  ConsumeTest('E',4);
  ConsumeTest('l',5);
  ConsumeTest('d',6);
  ConsumeTest('8',7);
  ConsumeTest('I',8);
  nConsumeTest('I',9);
  ConsumeTest('7',10);
  ConsumeTest('U',11);
  nConsumeTest('f',12);
  nConsumeTest('g',13);
  ConsumeTest('1',14);
  ConsumeTest('S',15);
  ConsumeTest('t',16);
  ConsumeTest('p',17);
  ConsumeTest('2',18);
  ConsumeTest('2',19);
  ConsumeTest('J',20);
  nConsumeTest('J',21);
  ConsumeTest('6',22);
  ConsumeTest(' ',23);
  nConsumeTest('\n',24);
  ConsumeTest(' ',25);
  ConsumeTest(' ',26);
  ConsumeTest('7',26);
  ConsumeTest('\n',28);
  ConsumeTest('\n',29);
  ConsumeTest('6',30);//*/
}
#undef nConsumeTest
#undef ConsumeTest
#define ConsumeTest(data, num) err.state=LexErr::Status::OK;if(st.TryConsumTerm( data , err).state != LexErr::Status::OK) ERR("TestTryConsumTerm", "Testing the consum therm method faild to consum "#num) 
#define nConsumTest(data, num) err.state=LexErr::Status::OK;if(st.TryConsumTerm( data , err).state != LexErr::Status::OK) ERR("TestTryConsumTerm", "Testing the consum therm method faild it wrongly consumed "#num" "#data) 
void TestTryConsumTerm(){
  Lex st("./UnitTest/TestData/RandomContent.test");
  LexErr err;
  nConsumTest('b',1);
  ConsumeTest('m',1);
  ConsumeTest('q',2);
  ConsumeTest('6',3);
  ConsumeTest('E',4);
  ConsumeTest('l',5);
  ConsumeTest('d',6);
  ConsumeTest('8',7);
  ConsumeTest('I',8);
  nConsumTest('I',9);
  ConsumeTest('c',9);
  ConsumeTest('7',10);
  nConsumTest('U',11);
  ConsumeTest('K',12);
  ConsumeTest('A',13);
  nConsumTest('1',14);
  ConsumeTest('S',15);
  ConsumeTest('t',16);
  ConsumeTest('p',17);
  ConsumeTest('2',18);
  ConsumeTest('2',19);
  ConsumeTest('J',20);
}
#undef nConsumTest
#undef ConsumeTest
#define ConsumeTest(data, num) err.state=LexErr::Status::OK;if(st.ConsumTerm<isdigit>( data , err).state != LexErr::Status::OK) ERR("TestConsumTermTemplate", "Testing the consum therm method faild to consum "#num) 
#define nConsumTest(data, num) err.state=LexErr::Status::OK;if(st.ConsumTerm<isdigit>( data , err).state != LexErr::Status::WARN) ERR("TestConsumTermTemplate", "Testing the consum therm method faild it wrongly consumed "#num" "#data) 
void TestConsumTermTemplate(){
  Lex st("./UnitTest/TestData/RandomContent.test");
  LexErr err;
  nConsumTest('b',1);
  ConsumeTest('m',1);
  ConsumeTest('q',2);
  ConsumeTest('6',3);
  ConsumeTest('E',4);
  ConsumeTest('l',5);
  ConsumeTest('d',6);
  ConsumeTest('d',7);
  nConsumTest('d',8);
  ConsumeTest('I',8);
  nConsumTest('I',9);
  ConsumeTest('c',9);
  ConsumeTest('7',10);
  ConsumeTest('U',11);
  ConsumeTest('K',12);
  ConsumeTest('A',13);
  ConsumeTest('1',14);
  ConsumeTest('S',15);
  ConsumeTest('t',16);
  ConsumeTest('p',17);
  ConsumeTest('2',18);
  ConsumeTest('2',19);
  ConsumeTest('J',20);
}
#undef nConsumTest
#undef ConsumeTest

void TestLexer(){
  //TestRemoveWhites();
  std::cout << "Runing TestLexWalk" << std::endl;
  TestLexWalk();


  std::cout << "Runing TestConsumTerm" << std::endl;
  TestConsumTerm();
  std::cout << "Runing TestTryConsumTerm" << std::endl;
  TestTryConsumTerm();
  std::cout << "Runing TestConsumTermTemplate" << std::endl;
  TestConsumTermTemplate();
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
