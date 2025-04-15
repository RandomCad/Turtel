cmake_minimum_required(VERSION 3.7)

project("Turtel")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 23)

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
antlr_target(CommandlineGrammarParser Commandline.g4 LISTENER OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/libs )

#---------------------------------------#
#             LLVM - CLANG              #
#---------------------------------------#
# Find LLVM and Clang
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using CLANG dir in: ${CLANG_INCLUDE_DIRS}")

# Add the include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

#LLVM libs
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

if(EXISTS "/etc/fedora-release")
  # Creat llvm-libs
  SET(LLVM_LIBS 
    clang-cpp
    LLVM
  ) 
# elseif(EXISTS "/etc/gentoo-release") 
else()
 # Creat llvm-libs
  SET(LLVM_LIBS 
    ${llvm_libs} 
    clangTooling 
    clangBasic 
    clangAST 
    clangDriver 
    clangFrontend 
    clangFrontendTool 
    clangSerialization
  )
endif()

#---------------------------------------#
#             Include SDL2              #
#---------------------------------------#

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(SDL2_LIBS ${SDL2_INCLUDE_DIRS})

#---------------------------------------#
#            Turtel Programm            #
#---------------------------------------#
# Creat CodeGenerator variable 
set(CodeGeneration 
  src/antlr.cpp 
  src/TopLevelVisitor.cpp
  src/VariableVisitor.cpp
  src/Function.cpp
  src/Variable.cpp
  src/GlobalVarVisitor.cpp
  src/CommandLineListener.cpp
  )
# Creat LLVM Interface variable
set(LLVM_Interface src/LLVMInterface.cpp)

add_executable(Turtel 
  ${ANTLR_SceneGrammarParser_CXX_OUTPUTS} 
  ${ANTLR_CommandlineGrammarParser_CXX_OUTPUTS} 
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
  UnitTest/antlr.cpp
  UnitTest/TestLLVMInterface.cpp
  UnitTest/Variable.cpp
  UnitTest/TopLevelVisitor.cpp
  UnitTest/TopLevelVisitor.Math.cpp
  UnitTest/TopLevelVisitor.Cond.cpp
  UnitTest/TopLevelVisitor.Loop.cpp
  UnitTest/TopLevelVisitor.Var.cpp
  UnitTest/TopLevelVisitor.Move.cpp
  UnitTest/TopLevelVisitor.IfEls.cpp
  UnitTest/TopLevelVisitor.End.cpp
  UnitTest/VariableVisitor.cpp
  UnitTest/Function.cpp
  UnitTest/TestHelper.cpp
  UnitTest/CommandListener.cpp
  )

add_executable(
  Tests
  ${UnitTest}
  ${ANTLR_SceneGrammarParser_CXX_OUTPUTS} 
  ${ANTLR_CommandlineGrammarParser_CXX_OUTPUTS} 
  ${CodeGeneration} 
  ${LLVM_Interface}
  ${SDL2_LIBS}
)

target_compile_definitions(Tests PRIVATE UNIT_TEST=1)

target_link_libraries(
  Tests
  GTest::gtest_main
  SDL2_image::SDL2_image
  ${SYSTEM_LIBS}
  antlr4_static
)

include(GoogleTest)
gtest_discover_tests(Tests)

# Ensure the UnitTest/TestData directory is copied BEFORE running the tests
add_custom_target(copy_test_data
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/UnitTest/TestData ${CMAKE_CURRENT_BINARY_DIR}/TestData
  COMMENT "Copying UnitTest/TestData to output directory"
)

# Ensure Tests depends on copy_test_data (copies test data before running tests)
add_dependencies(Tests copy_test_data)

# ----------------------------
# Doxygen Integration
# ----------------------------
find_package(Doxygen REQUIRED)

if (DOXYGEN_FOUND)
  set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
endif()
