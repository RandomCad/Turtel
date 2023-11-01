all: say_hello compile

SRCS_MAIN := $(wildcard *.cpp)
SRCS_LEX := $(wildcard ./Lexer/*.cpp)

SRCS := ${SRCS_MAIN} ${SRCS_LEX}

gpp = g++ -Wpedantic -fpermissive -Wall -O3

normDef = -DDEBUG

say_hello:
	@echo "Hello World"

compile:
	${gpp} ${normDef} ${SRCS} 

UDef = 
test:
	${gpp} ${UDef} ./UnitTest/UnitTest.cpp -o UTest.out
	./UTest.out
