# shaked1mi@gmail.com

CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Wextra -I.

SRC_MAIN   := main.cpp
SRC_TEST   := Test.cpp

TARGET_MAIN := main
TARGET_TEST := test

.PHONY: all main test valgrind clean

all: main test

main: $(SRC_MAIN) MyContainer.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_MAIN) $(SRC_MAIN)

test: $(SRC_TEST) MyContainer.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_TEST) $(SRC_TEST)

valgrind: test
	valgrind --leak-check=full ./$(TARGET_TEST)
	valgrind --leak-check=full ./$(TARGET_MAIN)

clean:
	rm -f $(TARGET_MAIN) $(TARGET_TEST) *.o
