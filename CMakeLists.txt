cmake_minimum_required(VERSION 3.7)

project("Turtel")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 20)

#emit clangd data
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(NOT WIN32)
  FIND_PACKAGE(Threads REQUIRED)
endif()

if (NOT WIN32)
  SET( SYSTEM_LIBS ${SYSTEM_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
endif()

if( WIN32)
  SET( SYSTEM_LIBS  ${SYSTEM_LIBS}  gdi32 )
endif()

# Add CIMG Flags to Compilation Flags
SET(CMAKE_CXX_FLAGS "-Wall -O3")

#---------------------------------------#
#                 ANTLR                 #
#---------------------------------------#
# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr-4.13.1-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(SceneGrammarParser Scene.g4 VISITOR LISTENER OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/libs )

#---------------------------------------#
#             LLVM - CLANG              #
#---------------------------------------#
if(EXISTS "/etc/fedora-release")
  message(STATUS "Fedora erkannt: Fedora-spezifische LLVM/CLANG-Einstellungen werden verwendet")
  find_package(LLVM REQUIRED CONFIG)
  find_package(Clang REQUIRED CONFIG)
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
  message(STATUS "Using CLANG dir in: ${CLANG_INCLUDE_DIRS}")
  include_directories(${LLVM_INCLUDE_DIRS})
  include_directories(${CLANG_INCLUDE_DIRS})
  add_definitions(${LLVM_DEFINITIONS})
  separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
  add_definitions(${LLVM_DEFINITIONS_LIST})
elseif(EXISTS "/etc/gentoo-release")
  message(STATUS "Gentoo erkannt: Gentoo-spezifische LLVM/CLANG-Einstellungen werden verwendet")
  find_package(LLVM REQUIRED CONFIG)
  find_package(Clang REQUIRED CONFIG)
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
  message(STATUS "Using CLANG dir in: ${CLANG_INCLUDE_DIRS}")
  include_directories(${LLVM_INCLUDE_DIRS})
  include_directories(${CLANG_INCLUDE_DIRS})
  add_definitions(${LLVM_DEFINITIONS})
  separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
  add_definitions(${LLVM_DEFINITIONS_LIST})
else()
  message(STATUS "Andere Linux-Distribution erkannt: Standard LLVM/CLANG-Einstellungen werden verwendet")
  find_package(LLVM REQUIRED CONFIG)
  find_package(Clang REQUIRED CONFIG)
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
  message(STATUS "Using CLANG dir in: ${CLANG_INCLUDE_DIRS}")
  include_directories(${LLVM_INCLUDE_DIRS})
  include_directories(${CLANG_INCLUDE_DIRS})
  add_definitions(${LLVM_DEFINITIONS})
  separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
  add_definitions(${LLVM_DEFINITIONS_LIST})
endif()

#LLVM libs
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Creat llvm-libs
SET(LLVM_LIBS 
  clang-cpp
  LLVM
  ) 

#---------------------------------------#
#            Turtel Programm            #
#---------------------------------------#
# Creat CodeGenerator variable 
set(CodeGeneration 
  src/TopLevelVisitor.cpp
  src/VariableVisitor.cpp
  src/Function.cpp
  src/Variable.cpp
  src/GlobalVarVisitor.cpp
  )
# Creat LLVM Interface variable
set(LLVM_Interface src/LLVMInterface.cpp)

add_executable(Turtel 
  src/antlr.cpp 
  ${ANTLR_SceneGrammarParser_CXX_OUTPUTS} 
  ${CodeGeneration} 
  ${LLVM_Interface}
  )

SET( SYSTEM_LIBS  ${SYSTEM_LIBS} ${LLVM_LIBS})

# link required libraries
target_link_libraries(Turtel antlr4_static ${SYSTEM_LIBS})

#---------------------------------------#
#           UnitTest Programm           #
#---------------------------------------#
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(
  UnitTest
  UnitTest/TestLLVMInterface.cpp
  UnitTest/Variable.cpp
  UnitTest/TopLevelVisitor.cpp
  UnitTest/VariableVisitor.cpp
  UnitTest/Function.cpp
  UnitTest/TestHelper.cpp
  )

add_executable(
  Tests
  ${UnitTest}
  ${ANTLR_SceneGrammarParser_CXX_OUTPUTS} 
  ${CodeGeneration} 
  ${LLVM_Interface}
)
target_compile_definitions(Tests PRIVATE UNIT_TEST=1)
target_link_libraries(
  Tests
  GTest::gtest_main
  ${SYSTEM_LIBS}
  antlr4_static
)

include(GoogleTest)
gtest_discover_tests(Tests)

