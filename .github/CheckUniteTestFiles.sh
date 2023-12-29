#! /bin/bash

echo 'Checking ./UnitTest/TestData/blanck.test'
echo "da35999de689c8ea2e2e3b88e57ed24e02b7eb444c3cb2b5aa99ad90280cc76e ./UnitTest/TestData/blanck.test" | sha256sum --check --status 
if (($? != 0)); then
  exit 1
fi

echo 'Checking ./UnitTest/TestData/WhiteFollowedByJiber.test'
echo "a09f3b877fe7323ff717e3894c7f87c365aedcdac602a1599ab80affe736e036  ./UnitTest/TestData/WhiteFollowedByJiber.test" | sha256sum --check --status 
if (($? != 0)); then
  exit 1
fi

echo 'Checking ./UnitTest/TestData/./UnitTest/TestData/Walk.test'
echo "1a90e2a773aa08df2123f6e8810bd9d9ada29dd017e132f2b62c74edcd21f10e  ./UnitTest/TestData/Walk.test" | sha256sum --check --status 
if (($? != 0)); then
  exit 1
fi

echo 'Checking ./UnitTest/TestData/./UnitTest/TestData/RandomContent.test'
echo "c0df70e17907c3f24e0fabf10549919420352a1ee840465bfc6d9bc887366f7c  UnitTest/TestData/RandomContnt.test" | sha256sum --check --status 
if (($? != 0)); then
  exit 1
fi
