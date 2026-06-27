#include "address_resolver.hpp"
#include <netdb.h>
#include <iostream>
#include "resolved_addresses.hpp"
#include "socket.hpp"
#include <sys/socket.h>
#include <sys/socket.h>
#include <string>
#include <vector>
int main()
{
    address_resolver resolver;
    resolved_addresses address = resolver.resolve("127.0.0.1", "8080");
    socket_handler socket = socket_handler::create_socket(address.get());
    socket.connect_socket(*address.get());
    std::string message = "make me say no";
    socket.send_data(message);

    std::vector<char> buffer(1024);
    std::size_t received = socket.recv_data(buffer.data(), buffer.size());
    std::cout.write(buffer.data(), static_cast<std::streamsize>(received));
    std::cout << '\n';
}