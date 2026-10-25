CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude -pthread

LIB = $(shell find lib -name '*.cpp')
HEADERS = $(shell find include -name '*.h')
MAIN_SRC = src/main.cpp
ADMIN_SRC = src/util/admin.cpp
MATRIX_MULTIPLICATION_SRC = src/util/matrix_multiplication.cpp
GAME_SRC = src/util/game.cpp
GAME_SERVER_SRC = src/util/game_server.cpp
GAME_CLIENT_SRC = src/util/game_client.cpp
PALINDROME_SRC = src/util/palindrome.cpp
FX_SRC = src/util/fx.cpp
COUNT_SRC = src/util/count.cpp
INDEXER_SRC = src/util/indexer.cpp

TARGET = target/sistope
ADMIN_TARGET = target/util/admin
MATRIX_MULTIPLICATION_TARGET = target/util/matrix_multiplication
GAME_TARGET = target/util/game
GAME_SERVER_TARGET = target/util/game_server
GAME_CLIENT_TARGET = target/util/game_client
PALINDROME_TARGET = target/util/palindrome
FX_TARGET = target/util/fx
COUNT_TARGET = target/util/count
INDEXER_TARGET = target/util/indexer


all: compile

mkdir:
	mkdir -p target/util 

compile: $(TARGET) $(ADMIN_TARGET) $(MATRIX_MULTIPLICATION_TARGET) $(GAME_TARGET) $(GAME_SERVER_TARGET) $(GAME_CLIENT_TARGET) $(PALINDROME_TARGET) $(FX_TARGET) $(COUNT_TARGET) $(INDEXER_TARGET)

$(TARGET): $(MAIN_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(LIB) -o $(TARGET)

$(ADMIN_TARGET): $(ADMIN_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(ADMIN_SRC) $(LIB) -o $(ADMIN_TARGET)

$(MATRIX_MULTIPLICATION_TARGET): $(MATRIX_MULTIPLICATION_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(MATRIX_MULTIPLICATION_SRC) $(LIB) -o $(MATRIX_MULTIPLICATION_TARGET)

$(GAME_TARGET): $(GAME_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(GAME_SRC) $(LIB) -o $(GAME_TARGET)

$(GAME_SERVER_TARGET): $(GAME_SERVER_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(GAME_SERVER_SRC) $(LIB) -o $(GAME_SERVER_TARGET)

$(GAME_CLIENT_TARGET): $(GAME_CLIENT_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(GAME_CLIENT_SRC) $(LIB) -o $(GAME_CLIENT_TARGET)

$(PALINDROME_TARGET): $(PALINDROME_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(PALINDROME_SRC) $(LIB) -o $(PALINDROME_TARGET)

$(FX_TARGET): $(FX_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(FX_SRC) $(LIB) -o $(FX_TARGET)

$(COUNT_TARGET): $(COUNT_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(COUNT_SRC) $(LIB) -o $(COUNT_TARGET)

$(INDEXER_TARGET): $(INDEXER_SRC) $(LIB) $(HEADERS) | mkdir
	$(CXX) $(CXXFLAGS) $(INDEXER_SRC) $(LIB) -o $(INDEXER_TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(ADMIN_TARGET) $(MATRIX_MULTIPLICATION_TARGET) $(GAME_TARGET) $(GAME_SERVER_TARGET) $(GAME_CLIENT_TARGET) $(PALINDROME_TARGET) $(FX_TARGET) $(COUNT_TARGET) $(INDEXER_TARGET)

.PHONY: all run clean mkdir compile
