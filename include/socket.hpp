#pragma once


#include <sys/socket.h>
#include <sys/types.h>
#include "resolved_addresses.hpp"
#include <stdexcept>
#include <cstddef>//byte view size_t
#include <string>
#include "udp_datagram.hpp"



class socket_handler
{
public:

    explicit socket_handler(int fd_);
    socket_handler(const socket_handler&) = delete;
    socket_handler& operator=(const socket_handler&) = delete;
    socket_handler(socket_handler&& other) noexcept;
    socket_handler& operator=(socket_handler&& other) noexcept;
    ~socket_handler();

    static socket_handler create_socket(addrinfo* adresses);
    void bind_socket(addrinfo& adersses);
    void connect_socket(addrinfo& addresses);
    void socket_listen(int backlog);
    socket_handler socket_accept();

    int get_fd(){
        return fd_;
    }

    std::size_t send_data(const char* msg);
    std::size_t send_data(const std::string& msg);
    std::size_t send_data(const void* msg, std::size_t size);

    std::size_t recv_data(void* msg, std::size_t size);

    std::size_t send_data_to(const void* msg, std::size_t size, const sockaddr* address, socklen_t address_len);
    std::size_t send_data_to(const void* msg, std::size_t size, const udp_datagram& datagram);
    std::size_t send_data_to(const void* msg,std::size_t size,addrinfo* address);
    
    
    
    udp_datagram recv_data_from(void* msg, std::size_t size);
    std::size_t recv_data_from(void* msg, std::size_t size, sockaddr* address, socklen_t* address_len);

    void set_nonblocking();

private:

    int fd_;

};