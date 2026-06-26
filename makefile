CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -Iinclude
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OUTPUT = main

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(OUTPUT)

run: $(OUTPUT)
	./$(OUTPUT)

clean:
	rm -f $(OUTPUT)

.PHONY: all run clean
