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

# CMake commands copied by the CMake file for CImg

# To use PKG_CHECK_MODULES to find some optional packages

if(NOT WIN32)
  FIND_PACKAGE(Threads REQUIRED)
endif()

# #### End of additional libraries search ##########



if (NOT WIN32)
  SET( SYSTEM_LIBS ${SYSTEM_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
endif()

if( WIN32)
  SET( SYSTEM_LIBS  ${SYSTEM_LIBS}  gdi32 )
endif()

# Add CIMG Flags to Compilation Flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CIMG_CFLAGS}")
SET(CMAKE_CXX_FLAGS "-Wall -O3")

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


# Call macro to add lexer and grammar to your build dependencies.
antlr_target(SceneGrammarParser Scene.g4 VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}
					 ${PROJECT_SOURCE_DIR}/libs/
  )
  
set(VISITOR CodeGenerator.cpp CodeGenerator.Helper.cpp)

add_executable(Turtel antlr.cpp ${VISITOR}  ${ANTLR_SceneGrammarParser_CXX_OUTPUTS})


# link required libraries
target_link_libraries(Turtel antlr4_static ${SYSTEM_LIBS})

# copy the input file next to the generated executable
add_custom_command(TARGET Turtel POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${PROJECT_SOURCE_DIR}/input.scene"
  $<TARGET_FILE_DIR:Turtel>)

# necessary because the default working directory of Visual Studio
# is not the same as the one in which the binary is created
# so we copy the input file twice:
# one for when you launch the executale in Visual Studio (CTRL+F5)
# one for when you launch it normally
if (WIN32)
add_custom_command(TARGET Turtel POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${PROJECT_SOURCE_DIR}/input.scene"
  $<TARGET_FILE_DIR:Turtel>/../)
endif()

#Testing
include(CTest)

set(UnitTest UnitTest/TestExpr.cpp UnitTest/TestCodeGenerator.Helper.cpp)
# Compilation and linking
add_executable(tests UnitTest.cpp CodeGenerator.cpp ${UnitTest} ${VISITOR} ${ANTLR_SceneGrammarParser_CXX_OUTPUTS})
target_link_libraries(tests antlr4_static ${SYSTEM_LIBS})

add_test(TestNumberParsing tests COMMAND 0)
add_test(TestVariableParsing tests COMMAND 1)
add_test(TestExprParsing tests COMMAND 2)
add_test(TestWalkParsing tests COMMAND 3)
add_test(TestCodeGeneratorHelperMethodes tests COMMAND 4)
add_test(TestCodeGeneratorCDTor tests COMMAND 5)
