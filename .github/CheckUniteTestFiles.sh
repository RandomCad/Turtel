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
echo "dae86d88d3c2fcc6ef17ae471a956faac8690ad32f58a2471e774f5c4f60283f  ./UnitTest/TestData/Walk.test" | sha256sum --check --status 
if (($? != 0)); then
  exit 1
fi

echo 'Checking ./UnitTest/TestData/./UnitTest/TestData/RandomContent.test'
echo "c0df70e17907c3f24e0fabf10549919420352a1ee840465bfc6d9bc887366f7c  UnitTest/TestData/RandomContent.test" | sha256sum --check --status 
if (($? != 0)); then
  exit 1
fi
