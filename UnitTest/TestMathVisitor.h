#pragma once
#ifdef UNIT_TEST
#include "UnitTest.h"

bool TestNumberParsing(TestError*&);
bool TestNumberExec(TestError*&);
bool TestVariableParsing(TestError *&ret);
bool TestWalkParsing(TestError *&ret);

#endif
