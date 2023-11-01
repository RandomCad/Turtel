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


TEST(TestWalkLex)
#define PREF TestWalkLex()
  return 0;

}

TEST(main)
  return 0;
}
