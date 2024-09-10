cmake_minimum_required(VERSION 3.7)

project("UnitTest")

if(NOT WIN32)
  FIND_PACKAGE(Threads REQUIRED)
endif()

# LLVM

# Find LLVM and Clang
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

# Add the include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})





# #### End of additional libraries search ##########



if (NOT WIN32)
  SET( SYSTEM_LIBS ${SYSTEM_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
endif()

if( WIN32)
  SET( SYSTEM_LIBS  ${SYSTEM_LIBS}  gdi32 )
endif()

# Add CIMG Flags to Compilation Flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CIMG_CFLAGS}")
SET(CMAKE_CXX_FLAGS "-Wall -O3 -DUNIT_TEST")

# CMake commands for this specific project start here

# to disable some warnings for CImg
if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "-Wdeprecated -Wno-attributes" )
endif()

if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wdeprecated -std=c++11" )
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS "/EHsc /wd4127 /wd4311 /wd4312 /wd4512 /wd4571 /wd4640 /wd4706 /wd4710 /wd4800 /wd4804 /wd4820 /wd4996")
endif()
  
set(VISITOR ../src/CodeGenerator.cpp ../src/CodeGenerator.Helper.cpp ../src/LLVMInterface.cpp)



#LLVM libs
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

SET( SYSTEM_LIBS  ${SYSTEM_LIBS}  ${llvm_libs} clangTooling clangBasic clangAST clangDriver clangFrontend clangFrontendTool clangSerialization)



#Testing
include(CTest)
add_definitions(-DUNIT_TEST)

set(UnitTest TestExpr.cpp TestCodeGenerator.Helper.cpp TestLLVMInterface.cpp)
# Compilation and linking
add_executable(tests UnitTest.cpp ${UnitTest} ${ANTLER_GEN} ${VISITOR} ${ANTLR_SceneGrammarParser_CXX_OUTPUTS})
message(Antler Unit="${ANTLR_SceneGrammarParser_CXX_OUTPUTS}")

target_link_libraries(tests antlr4_static ${SYSTEM_LIBS})

add_test(TestUnitTestEngin ${CMAKE_CURRENT_SOURCE_DIR}/TestErrorCreatin.sh)
add_test(TestNumberParsing tests COMMAND 0)
add_test(TestVariableParsing tests COMMAND 1)
add_test(TestExprParsing tests COMMAND 2)
add_test(TestWalkParsing tests COMMAND 3)
#add_test(TestCodeGeneratorHelperMethodes tests COMMAND 4)
#add_test(TestCodeGeneratorCDTor tests COMMAND 5)
add_test(TestLLVMInterface tests COMMAND 6)
add_test(TestEmptyCodeGenerator tests COMMAND 7)
add_test(TestBasicTurtelProgramms tests COMMAND 8)
