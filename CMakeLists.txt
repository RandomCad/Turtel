cmake_minimum_required(VERSION 3.7)

project("Turtel")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 17)

#emit clangd data
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

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

# To use PKG_CHECK_MODULES to find some optional packages

if(NOT WIN32)
  FIND_PACKAGE(Threads REQUIRED)
endif()

add_subdirectory(src/)
#add_subdirectory(UnitTest/)

