#include "socket.hpp"
#include <unistd.h>
#include <cerrno>
#include <stdexcept>
#include <cstring>
#include <string>
#include <fcntl.h>

namespace
{
std::runtime_error make_socket_error(const char* operation)
{
    return std::system_error(errno,std::generic_category(),operation);
}
}

socket_handler::socket_handler(int fd) : fd_(fd) {}

socket_handler::socket_handler(socket_handler&& other) noexcept : fd_(other.fd_)
{
    other.fd_ = -1;
}

socket_handler& socket_handler::operator=(socket_handler&& other) noexcept
{
    if (this != &other)
    {
        if (fd_ != -1)
            close(fd_);

        fd_ = other.fd_;
        other.fd_ = -1;
    }

    return *this;
}

socket_handler::~socket_handler()
{
    if (fd_ != -1)
        close(fd_);
}

socket_handler socket_handler::create_socket(addrinfo* info)
{
    if (info == nullptr)
        throw std::invalid_argument("create_socket: null addrinfo");

    int fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (fd == -1)
    {
        throw make_socket_error("socket");
    }
    return socket_handler(fd);
}

void socket_handler::bind_socket(addrinfo& info){
    if (bind(fd_, info.ai_addr, info.ai_addrlen) == -1)
        throw make_socket_error("bind");
}

void socket_handler::connect_socket(addrinfo& info){
    if (connect(fd_, info.ai_addr, info.ai_addrlen) == -1)
        throw make_socket_error("connect");
}

void socket_handler::socket_listen(int backlog){
    if (listen(fd_, backlog) == -1)
        throw make_socket_error("listen");
}

socket_handler socket_handler::socket_accept(){
    sockaddr_storage client_address{};
    socklen_t client_address_length = sizeof(client_address);
    int fd = accept(fd_, reinterpret_cast<sockaddr*>(&client_address), &client_address_length);
    if (fd == -1)
        throw make_socket_error("accept");

    return socket_handler(fd);
    
}

std::size_t socket_handler::send_data(const char* msg)
{
    if (msg == nullptr)
        throw std::invalid_argument("send_data: null message");

    return send_data(msg, std::strlen(msg));
}

std::size_t socket_handler::send_data(const std::string& msg)
{
    return send_data(msg.data(), msg.size());
}

std::size_t socket_handler::send_data(const void* msg, std::size_t size)
{
    const auto* buffer = static_cast<const char*>(msg);
    std::size_t total_sent = 0;

    while (total_sent < size)
    {
        ssize_t sent = send(fd_, buffer + total_sent, size - total_sent, 0);
        if (sent == -1)
        {
            if (errno == EINTR)
                continue;

            throw make_socket_error("send");
        }

        total_sent += static_cast<std::size_t>(sent);
    }

    return total_sent;
}

std::size_t socket_handler::recv_data(void* msg, std::size_t size)
{
    auto* buffer = static_cast<char*>(msg);

    for (;;)
    {
        ssize_t rcvd = recv(fd_, buffer, size, 0);
        if (rcvd == -1)
        {
            if (errno == EINTR)
                continue;

            throw make_socket_error("recv");
        }

        return static_cast<std::size_t>(rcvd);
    }
}

std::size_t socket_handler::send_data_to(const void* msg, std::size_t size, const sockaddr* address, socklen_t address_len)
{
    ssize_t sent = sendto(fd_, msg, size, 0, address, address_len);
    if (sent == -1)
        throw make_socket_error("sendto");

    return static_cast<std::size_t>(sent);
}

std::size_t socket_handler::send_data_to(const void* msg, std::size_t size, const udp_datagram& datagram)
{
    return send_data_to(
        msg,
        size,
        reinterpret_cast<const sockaddr*>(datagram.sender.address()),
        datagram.sender.length());
}

std::size_t socket_handler::send_data_to(const void* msg, std::size_t size, addrinfo* address)
{
    if (address == nullptr)
        throw std::invalid_argument("send_data_to: null addrinfo");

    return send_data_to(msg, size, address->ai_addr, address->ai_addrlen);
}

udp_datagram socket_handler::recv_data_from(void* msg, std::size_t size)
{
    sockaddr_storage sender{};
    socklen_t length = sizeof(sender);

    ssize_t rcvd = recvfrom(
        fd_,
        msg,
        size,
        0,
        reinterpret_cast<sockaddr*>(&sender),
        &length);

    if (rcvd == -1)
        throw make_socket_error("recvfrom");

    auto* bytes = static_cast<std::byte*>(msg);
    return udp_datagram(
        std::vector<std::byte>(bytes, bytes + rcvd),
        endpoint(std::move(sender), length));
}

std::size_t socket_handler::recv_data_from(void* msg, std::size_t size, sockaddr* address, socklen_t* address_len)
{
    ssize_t rcvd = recvfrom(fd_, msg, size, 0, address, address_len);
    if (rcvd == -1)
        throw make_socket_error("recvfrom");

    return static_cast<std::size_t>(rcvd);
}


void socket_handler::set_nonblocking(){
    int flags=fcntl(fd_,F_GETFL,0);
    if(flags ==-1)
    throw make_socket_error("fcntl F_GETFL");
    if(fcntl(fd_,F_SETFL,flags | O_NONBLOCK) == -1 )
    throw make_socket_error("fcntl F_SETFL");
}