#include "address_resolver.hpp"
#include <netdb.h>
#include <iostream>
#include "resolved_addresses.hpp"
#include "socket.hpp"
#include <sys/socket.h>
#include <sys/socket.h>

int main(){
    address_resolver resolver;
    resolved_addresses address = resolver.resolve("127.0.0.1","8080");
    socket_handler socket = socket_handler::create_socket(address.get());
    addrinfo* addr = address.get();
    socket.bind_socket(*addr);
    socket.socket_listen(5);
    socket.socket_accept();
}