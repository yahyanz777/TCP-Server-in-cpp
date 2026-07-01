CXX = g++
CXXFLAGS = -std=c++20 -Wall -g -Iinclude -pthread
SRCDIR = src
COMMON_SOURCES = $(filter-out $(SRCDIR)/server.cpp $(SRCDIR)/client.cpp,$(wildcard $(SRCDIR)/*.cpp))
SERVER_SOURCES = tcpserver.cpp $(COMMON_SOURCES)
CLIENT_SOURCES = $(SRCDIR)/client.cpp $(COMMON_SOURCES)
SERVER = server
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $(SERVER_SOURCES) -o $(SERVER)
	chmod +x $(SERVER)

$(CLIENT): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $(CLIENT_SOURCES) -o $(CLIENT)
	chmod +x $(CLIENT)

run-server: $(SERVER)
	chmod +x $(SERVER)
	./$(SERVER)

run-client: $(CLIENT)
	chmod +x $(CLIENT)
	./$(CLIENT)

run: run-server

clean:
	rm -f $(SERVER) $(CLIENT)

.PHONY: all run run-server run-client clean
