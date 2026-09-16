CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

LIB = $(shell find lib -name '*.cpp')
MAIN_SRC = src/main.cpp
ADMIN_SRC = src/util/admin.cpp
MATRIX_MULTIPLICATION_SRC = src/util/matrix_multiplication.cpp
GAME_SRC = src/util/game.cpp
PALINDROME_SRC = src/util/palindrome.cpp
FX_SRC = src/util/fx.cpp
COUNT_SRC = src/util/count.cpp

TARGET = target/sistope
ADMIN_TARGET = target/util/admin
MATRIX_MULTIPLICATION_TARGET = target/util/matrix_multiplication
GAME_TARGET = target/util/game
PALINDROME_TARGET = target/util/palindrome
FX_TARGET = target/util/fx
COUNT_TARGET = target/util/count


all: compile

mkdir:
	mkdir -p target/util 

compile: $(TARGET) $(ADMIN_TARGET) $(MATRIX_MULTIPLICATION_TARGET) $(GAME_TARGET) $(PALINDROME_TARGET) $(FX_TARGET) $(COUNT_TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(LIB) -o $(TARGET)

$(ADMIN_TARGET):
	$(CXX) $(CXXFLAGS) $(ADMIN_SRC) $(LIB) -o $(ADMIN_TARGET)

$(MATRIX_MULTIPLICATION_TARGET):
	$(CXX) $(CXXFLAGS) $(MATRIX_MULTIPLICATION_SRC) $(LIB) -o $(MATRIX_MULTIPLICATION_TARGET)

$(GAME_TARGET):
	$(CXX) $(CXXFLAGS) $(GAME_SRC) $(LIB) -o $(GAME_TARGET)

$(PALINDROME_TARGET):
	$(CXX) $(CXXFLAGS) $(PALINDROME_SRC) $(LIB) -o $(PALINDROME_TARGET)

$(FX_TARGET):
	$(CXX) $(CXXFLAGS) $(FX_SRC) $(LIB) -o $(FX_TARGET)

$(COUNT_TARGET):
	$(CXX) $(CXXFLAGS) $(COUNT_SRC) $(LIB) -o $(COUNT_TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(ADMIN_TARGET) $(MATRIX_MULTIPLICATION_TARGET) $(GAME_TARGET) $(PALINDROME_TARGET) $(FX_TARGET) $(COUNT_TARGET)

.PHONY: all run clean
