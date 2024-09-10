cmake_minimum_required(VERSION 3.7)

project("Turtel")

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
antlr_target(SceneGrammarParser Scene.g4 VISITOR OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}
					 ${CMAKE_SOURCE_DIR}/libs/
  )
  
set(VISITOR CodeGenerator.cpp CodeGenerator.Helper.cpp LLVMInterface.cpp)

add_executable(Turtel antlr.cpp ${VISITOR}  ${ANTLR_SceneGrammarParser_CXX_OUTPUTS})

message(Antler src="${ANTLR_SceneGrammarParser_CXX_OUTPUTS}")

#LLVM libs
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

SET( SYSTEM_LIBS  ${SYSTEM_LIBS}  ${llvm_libs} clangTooling clangBasic clangAST clangDriver clangFrontend clangFrontendTool clangSerialization)



# link required libraries
target_link_libraries(Turtel antlr4_static ${SYSTEM_LIBS})

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
