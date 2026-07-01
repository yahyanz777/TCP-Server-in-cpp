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
        addrinfo* addr = address.get();
        socket.bind_socket(*addr);

        while (true)
        {
            std::vector<std::byte> buffer(1024);
            udp_datagram datagram = socket.recv_data_from(
                buffer.data(),
                buffer.size()
               );

            if (datagram.size == 0)
                continue;

            std::string reply(reinterpret_cast<char*>(buffer.data()), datagram.size);
            for (char& ch : reply)
            {
                ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
            }

            socket.send_data_to(
                reply.data(),
                reply.size(),
                datagram);
        }
        
        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}