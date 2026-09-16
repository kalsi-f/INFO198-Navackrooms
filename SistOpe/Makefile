CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

LIB = $(shell find lib -name '*.cpp')

TARGET = sistope
ADMIN_TARGET = admin
MATRIX_MULTIPLICATION_TARGET = matrix_multiplication

all: $(TARGET)

$(TARGET): main.o $(LIB)
	$(CXX) $(CXXFLAGS) main.o $(LIB) -o target/$(TARGET)

$(ADMIN_TARGET): admin_target.o $(LIB)
	$(CXX) $(CXXFLAGS) $(ADMIN_TARGET).o $(LIB) -o target/util/$(ADMIN_TARGET)

$(MATRIX_MULTIPLICATION_TARGET): matrix_multiplication.o $(LIB)
	$(CXX) $(CXXFLAGS) $(MATRIX_MULTIPLICATION_TARGET).o $(LIB) -o target/util/$(MATRIX_MULTIPLICATION_TARGET)

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

admin.o: src/util/$(ADMIN_TARGET).cpp
	$(CXX) $(CXXFLAGS) -c src/util/$(ADMIN_TARGET).cpp -o $(ADMIN_TARGET).o

matrix_multiplication.o: src/util/$(MATRIX_MULTIPLICATION_TARGET).cpp
	$(CXX) $(CXXFLAGS) -c src/util/$(MATRIX_MULTIPLICATION_TARGET).cpp -o $(MATRIX_MULTIPLICATION_TARGET).o

run: all
	./$(TARGET)

clean:
	rm -rf target

.PHONY: all run clean
