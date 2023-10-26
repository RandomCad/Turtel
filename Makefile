all: say_hello compile

SRCS_MAIN := $(wildcard *.cpp)
SRCS_LEX := $(wildcard ./Lexer/*.cpp)

SRCS := ${SRCS_MAIN} ${SRCS_LEX}

gpp = g++ -Wpedantic -fpermissive -Wall -DDEBUG

say_hello:
	@echo "Hello World"

compile:
	${gpp} ${SRCS} 


