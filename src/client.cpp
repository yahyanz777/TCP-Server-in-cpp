#include "address_resolver.hpp"
#include "resolved_addresses.hpp"
#include "socket.hpp"

int main()
{
    try
    {
        address_resolver resolver;
        resolved_addresses address = resolver.resolve("127.0.0.1", "8080");
        socket_handler socket = socket_handler::create_socket(address.get());

        std::string message = "make me say no";
        socket.send_data_to(
            message.data(),
            message.size(),
            address.get());

        std::vector<std::byte> buffer(1024);
        udp_datagram datagram = socket.recv_data_from(buffer.data(), buffer.size());
        std::cout<<datagram.text();
        std::cout << '\n';

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}