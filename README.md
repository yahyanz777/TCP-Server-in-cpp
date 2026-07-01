# High-Performance TCP Chat Server

A high-performance TCP chat server and client written in C++20 to explore modern Linux network programming. The project focuses on building an efficient asynchronous server from scratch using low-level POSIX socket APIs.

## Features

- **Asynchronous, non-blocking I/O** using `O_NONBLOCK`.
- **Edge-triggered `epoll`** for efficiently handling many concurrent connections.
- **Modern C++ design**
  - RAII wrappers for sockets and `epoll`
  - Move semantics for safe resource ownership
  - `std::system_error`-based exception handling
- **Multithreaded client** using `std::thread` and `std::atomic` for simultaneous sending and receiving.

## Tech Stack

- **Language:** C++20
- **Platform:** Linux
- **Build System:** Make
- **Libraries/APIs:** POSIX Sockets, `epoll`, `getaddrinfo`

## Project Structure

```
.
├── tcpserver.cpp              # Server entry point
├── src/
│   ├── client.cpp             # Chat client
│   ├── socket.cpp             # Socket wrapper
│   ├── epoll_handler.cpp      # epoll wrapper
│   └── address_resolver.cpp   # DNS/address resolution
└── include/
```

## Build

```bash
make all
```

## Run

Start the server:

```bash
make run-server
# or
./server [host] [port]
```

Start the client:

```bash
make run-client
# or
./client [host] [port]
```

The server listens on `0.0.0.0:3000` by default, and the client connects to `127.0.0.1:3000`. After connecting, enter your name and start chatting. Use `/quit` to disconnect.