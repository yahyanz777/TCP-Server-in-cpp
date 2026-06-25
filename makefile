CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/address_resolver.cpp $(SRCDIR)/resolved_addresses.cpp $(SRCDIR)/address_resolution_error.cpp
OUTPUT = main

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(OUTPUT)

run: $(OUTPUT)
	./$(OUTPUT)

clean:
	rm -f $(OUTPUT)

.PHONY: all run clean
