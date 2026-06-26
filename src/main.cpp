#include "address_resolver.hpp"
#include <netdb.h>
#include <iostream>
#include "resolved_addresses.hpp"
#include "socket.hpp"
#include <sys/socket.h>
#include <sys/socket.h>

int main(){
    address_resolver resolver;
    resolved_addresses address = resolver.resolve("142.250.181.142","80");
    socket_handler socket = socket_handler::create_socket(address.get());
    socket_handler::bind_socket(socket,*address.get());
}