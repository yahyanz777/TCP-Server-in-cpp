#include "socket.hpp"
#include <unistd.h>
#include <cerrno>
#include <stdexcept>
#include <cstring>
socket_handler::socket_handler(int fd) : fd_(fd) {}

socket_handler::~socket_handler()
{
    if (fd_ != -1)
        close(fd_);
}

socket_handler socket_handler::create_socket(addrinfo* info)
{

    int fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (fd == -1)
    {
            throw std::runtime_error(std::strerror(errno));
    }
    return socket_handler(fd);
}

void socket_handler::bind_socket(socket_handler& socket,addrinfo& info){
if(bind(socket.fd_,info.ai_addr,info.ai_addrlen)==-1)
throw std::runtime_error(std::strerror(errno));
}

void socket_handler::connect_socket(socket_handler& socket,addrinfo& info){
    if(connect(socket.fd_,info.ai_addr,info.ai_addrlen)== -1 ){
        throw std::runtime_error(std::strerror(errno));
    }
}

void socket_handler::socket_listen(int backlog){
    if(listen(fd_,backlog)== -1)
        throw std::runtime_error(std::strerror(errno));
}

socket_handler socket_handler::socket_accept(){
    sockaddr_storage client_address{};
    socklen_t client_address_length = sizeof(client_address);
    int fd = accept(fd_, reinterpret_cast<sockaddr*>(&client_address), &client_address_length);
    if (fd == -1)
        throw std::runtime_error(std::strerror(errno));

    return socket_handler(fd);
}