#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include "resolved_addresses.hpp"
#include <stdexcept>

class socket_handler
{
public:
    explicit socket_handler(int fd_);
    ~socket_handler();
    static socket_handler create_socket(addrinfo* adresses);
    static void bind_socket(socket_handler& socket, addrinfo& adersses);
    static void connect_socket(socket_handler& socket,addrinfo& addresses);
    void socket_listen(int backlog);
    socket_handler socket_accept();
    
private:
int fd_;
};